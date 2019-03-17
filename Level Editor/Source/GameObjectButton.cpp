#include "stdafx.h"
#include "GameObjectButton.h"
#include <GameObject.h>
#include <Space.h>
#include "MenuController.h"
#include <Sprite.h>
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <SpriteSource.h>


GameObjectButton::GameObjectButton() : Button("GameObjectButton")
{
	menuController = nullptr;
	sprite = nullptr;
}

Component * GameObjectButton::Clone() const
{
	return new GameObjectButton(*this);
}

void GameObjectButton::Initialize()
{
	GameObject* MenuObject = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("MenuController");

	if (MenuObject != nullptr)
	{
		menuController = MenuObject->GetComponent<MenuController>();
	}

	sprite = GetOwner()->GetComponent<Sprite>();
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
	}

	delete go;
	go = nullptr;
}
