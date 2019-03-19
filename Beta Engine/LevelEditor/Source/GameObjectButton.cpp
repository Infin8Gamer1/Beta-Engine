#include "stdafx.h"
#include "GameObjectButton.h"
#include <GameObject.h>
#include <Space.h>
#include "MenuController.h"
#include <Sprite.h>
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <SpriteSource.h>
#include <SpriteText.h>
#include <Transform.h>

GameObjectButton::GameObjectButton() : Button("GameObjectButton")
{
	menuController = nullptr;
	sprite = nullptr;
	TextGO = nullptr;
	TextSprite = nullptr;
}

Component * GameObjectButton::Clone() const
{
	return new GameObjectButton(*this);
}

GameObjectButton::~GameObjectButton()
{
	if (TextGO != nullptr) {
		TextGO->Destroy();
	}	
}

void GameObjectButton::Initialize()
{
	GameObject* MenuObject = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("MenuController");

	if (MenuObject != nullptr)
	{
		menuController = MenuObject->GetComponent<MenuController>();
	}

	sprite = GetOwner()->GetComponent<Sprite>();

	if (TextGO != nullptr)
	{
		Transform* transform = TextGO->GetComponent<Transform>();

		transform->SetTranslation(GetOwner()->GetComponent<Transform>()->GetTranslation() + Vector2D(0, -35.0f));
		transform->SetScale(Vector2D(30, 30));

		GetOwner()->GetSpace()->GetObjectManager().AddObject(*TextGO);
	}
}

void GameObjectButton::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	Button::Update(dt);

	TextSprite->SetAlpha(sprite->GetAlpha());
}

void GameObjectButton::Clicked()
{
	if (menuController == nullptr)
	{
		Initialize();
	}

	menuController->SetSelectedGameObjectName(GameObjectName);
}

void GameObjectButton::SetGameObjectName(std::string Name)
{
	GameObjectName = Name;

	GameObject* go = GameObjectFactory::GetInstance().CreateObject(GameObjectName);

	if (go != nullptr)
	{
		Sprite* otherSprite = go->GetComponent<Sprite>();

		if (otherSprite != nullptr)
		{
			sprite = GetOwner()->GetComponent<Sprite>();
			sprite->SetSpriteSource(otherSprite->GetSpriteSource());
			sprite->RefreshAutoMesh();
			sprite->SetFrame(0);
		}

		TextGO = GameObjectFactory::GetInstance().CreateObject("LevelEditor/SpriteText");

		if (TextGO != nullptr)
		{
			TextSprite = TextGO->GetComponent<SpriteText>();

			TextSprite->SetText(go->GetName());
		}
		else {
			delete TextGO;
			TextGO = nullptr;
		}
	}

	delete go;
	go = nullptr;
}
