//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectFactory.h"
#include <GameObject.h>
#include <Parser.h>
#include <sstream>
#include <Transform.h>
#include <Sprite.h>
#include <ColliderCircle.h>
#include <ColliderPoint.h>
#include <ColliderRectangle.h>
#include <ColliderTilemap.h>
#include <Animation.h>
#include <Physics.h>
#include <SpriteTilemap.h>
#include <SpriteText.h>
#include <ColliderLine.h>

static GameObjectFactory* Instance;

GameObjectFactory::GameObjectFactory()
{
	objectFilePath = "Assets/Objects/";
	
	RegisterComponent<Transform>();
	RegisterComponent<Sprite>();
	RegisterComponent<SpriteTilemap>();
	RegisterComponent<SpriteText>();
	RegisterComponent<Animation>();
	RegisterComponent<Physics>();
	RegisterComponent<ColliderCircle>();
	RegisterComponent<ColliderPoint>();
	RegisterComponent<ColliderRectangle>();
	RegisterComponent<ColliderTilemap>();
	RegisterComponent<ColliderLine>();
}

GameObjectFactory::~GameObjectFactory()
{	
	std::vector<Component*>::iterator i;

	for (i = registeredComponents.begin(); i != registeredComponents.end(); ++i)
	{
		delete (*i);
		*i = nullptr;
	}

	registeredComponents.clear();
	registeredComponents.shrink_to_fit();
}

GameObject * GameObjectFactory::CreateObject(const std::string & name, bool absolutePath, Mesh * mesh, SpriteSource * spriteSource)
{
	GameObject* GO = new GameObject(name);

	Parser* parser = nullptr;

	if (absolutePath)
	{
		parser = new Parser(name, std::fstream::in);
	} else {
		parser = new Parser(objectFilePath + name + ".object", std::fstream::in);
	}
	
	try
	{
		GO->Deserialize(*parser);

		Sprite* sprite = GO->GetComponent<Sprite>();

		if (sprite != nullptr) {
			if (mesh != nullptr) {
				sprite->SetMesh(mesh);
			}
			if (spriteSource != nullptr) {
				sprite->SetSpriteSource(spriteSource);
			}
		}
	}
	catch (ParseException e)
	{
		std::cout << e.what() << std::endl;

		delete GO;
		GO = nullptr;
	}

	delete parser;
	parser = nullptr;
	
	return GO;
}

Component * GameObjectFactory::CreateComponent(const std::string & name)
{
	for (size_t i = 0; i < registeredComponents.size(); i++)
	{
		std::string currentName = std::string(typeid(*registeredComponents[i]).name());

		std::istringstream stringStream(currentName);

		do
		{
			std::string inName;
			stringStream >> inName;

			if (name == inName && inName != "class") {
				return registeredComponents[i]->Clone();
			}
		} while (stringStream);
	}
	
	return nullptr;
}

void GameObjectFactory::SaveObjectToFile(GameObject * object)
{
	Parser* parser = new Parser(objectFilePath + object->GetName() + ".object", std::fstream::out);

	object->Serialize(*parser);

	delete parser;
	parser = nullptr;

	std::cout << "The Game Object: " << object->GetName() << " was saved to \"" << objectFilePath << object->GetName() << ".txt\"" << std::endl;
}

GameObjectFactory & GameObjectFactory::GetInstance()
{
	if (Instance == nullptr) {
		Instance = new GameObjectFactory();
	}

	return *Instance;
}


