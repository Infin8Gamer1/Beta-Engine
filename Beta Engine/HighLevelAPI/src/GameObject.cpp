//------------------------------------------------------------------------------
//
// File Name:	GameObject.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "Space.h"
#include <Parser.h>
#include <GameObjectFactory.h>

GameObject::GameObject(const std::string & name) : BetaObject(name)
{
	isDestroyed = false;
	damageHandler = nullptr;
	Health = 0;
	Path = name;
	gameObjectName = name;
}

GameObject::GameObject(const GameObject & other) : BetaObject(other.GetName())
{

	for (size_t i = 0; i < other.components.size(); i++)
	{
		Component* newComp = other.components[i]->Clone();

		if (newComp != nullptr) {
			AddComponent(newComp);
		}
		
	}
	
	isDestroyed = false;
	damageHandler = other.damageHandler;
	Health = other.Health;
	Path = other.Path;
}

GameObject::~GameObject()
{
	std::vector<Component*>::iterator i;

	for (i = components.begin(); i != components.end(); ++i)
	{
		delete (*i);
		*i = nullptr;
	}

	components.clear();
	components.shrink_to_fit();

	if (bar != nullptr)
	{
		TwDeleteBar(bar);
	}
}

void GameObject::Deserialize(Parser & parser)
{
	std::string mName = this->GetName();
	parser.ReadSkip(mName);
	parser.ReadSkip("{");

	unsigned numComponents = 0;
	parser.ReadVar(numComponents);

	for (unsigned i = 0; i < numComponents; i++)
	{
		std::string componentName;
		parser.ReadValue(componentName);

		Component* component = GameObjectFactory::GetInstance().CreateComponent(componentName);

		if (component == nullptr) {
			std::cout << "WARNING : Component " << componentName << " could not be found ... Skipping" << std::endl;

			std::string ref = parser.ReadSkipComponent();

			UnloadedComponentNames.push_back(componentName);
			UnloadedComponentStrings.push_back(ref);

			//throw ParseException(componentName, "Component " + componentName + " could not be found! ERROR 404");
		}
		else {
			AddComponent(component);

			parser.ReadSkip("{");
			component->Deserialize(parser);
			parser.ReadSkip("}");
		}
	}

	parser.ReadSkip("}");
}

void GameObject::DeserializeB(Parser & parser)
{
	parser.ReadSkip("{");

	unsigned numComponents = 0;
	parser.ReadVar(numComponents);

	for (unsigned i = 0; i < numComponents; i++)
	{
		std::string componentName;
		parser.ReadValue(componentName);

		Component* component = GameObjectFactory::GetInstance().CreateComponent(componentName);

		if (component == nullptr) {
			std::cout << "WARNING : Component " << componentName << " could not be found ... Skipping" << std::endl;

			std::string ref = parser.ReadSkipComponent();

			UnloadedComponentNames.push_back(componentName);
			UnloadedComponentStrings.push_back(ref);

			//throw ParseException(componentName, "Component " + componentName + " could not be found! ERROR 404");
		}
		else {
			AddComponent(component);

			parser.ReadSkip("{");
			component->Deserialize(parser);
			parser.ReadSkip("}");
		}
	}

	parser.ReadSkip("}");
}

void GameObject::Serialize(Parser & parser) const
{
	parser.WriteValue(GetName());

	parser.BeginScope();

	size_t numComponents = components.size() + UnloadedComponentNames.size();
	parser.WriteVar(numComponents);

	for (size_t i = 0; i < components.size(); i++)
	{
		//write components name
		parser.WriteValue(std::string(typeid(*components[i]).name()).substr(6));

		parser.BeginScope();

		components[i]->Serialize(parser);

		parser.EndScope();
	}

	for (size_t i = 0; i < UnloadedComponentNames.size(); i++)
	{
		parser.WriteValue(UnloadedComponentNames[i]);

		parser.WriteValue(UnloadedComponentStrings[i]);
	}

	parser.EndScope();
}

void TW_CALL DeleteCallback(void *clientData)
{
	// do something
	GameObject* go = static_cast<GameObject*>(clientData);

	go->Destroy();
}

TwBar* GameObject::CreateTweakBar(TwBar* bar_)
{
	bar = bar_;

	if (bar == nullptr) {
		std::string name = GetName() + " Properties";
		bar = TwNewBar(name.c_str());
		TwDefine(std::string(" '" + name + "' size = '200 300' position = '25 135' alpha = 128 refresh = 0.15 movable = true resizable = true contained = false color = '28 183 255'").c_str());
	}
	
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->AddVarsToTweakBar(bar);
	}

	TwAddButton(bar, "Delete Game Object", DeleteCallback, this, NULL);

	return bar;
}

void GameObject::Initialize()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Initialize();
	}
}

void GameObject::Update(float dt)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Update(dt);
	}
}

void GameObject::FixedUpdate(float dt)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->FixedUpdate(dt);
	}
}

void GameObject::Draw()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Draw();
	}
}

size_t GameObject::NumberOfComponents()
{
	return components.size();
}

void GameObject::AddComponent(Component * component)
{
	component->SetParent(this);
	components.push_back(component);
}

Component * GameObject::GetComponent(const std::string & _name)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i]->GetName() == _name)
		{
			return components[i];
		}
	}
	//std::cout << "No component with the name " + _name + " was found!" << std::endl;
	return nullptr;
}

void GameObject::Destroy()
{
	isDestroyed = true;
}

bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

Space * GameObject::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}

void GameObject::setDamageHandler(DamageEventHandler handler)
{
	damageHandler = handler;
}

void GameObject::DealDamage(GameObject & damegedGO, GameObject & damageCauser, int ammount, std::string damageType)
{
	if (damegedGO.damageHandler == nullptr) {
		return;
	}

	damegedGO.damageHandler(ammount, damageType, damageCauser, damegedGO);
}

int GameObject::getHealth()
{
	return Health;
}

void GameObject::setHealth(int _health) {
	Health = _health;
}

std::string GameObject::GetSavePath()
{
	return Path;
}

void GameObject::SetSavePath(std::string _Path)
{
	Path = _Path;
}

const std::string & GameObject::GetName() const
{
	return gameObjectName;
}

void GameObject::SetName(const std::string& newName)
{
	gameObjectName = newName;
}