#include "stdafx.h"
#include "TileButton.h"
#include <Sprite.h>
#include <Engine.h>
#include <Space.h>
#include <SpaceManager.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <SpriteTilemap.h>
#include "MenuController.h"

TileButton::TileButton() : Button("TileButton")
{
	menuController = nullptr;
}

Component * TileButton::Clone() const
{
	return new TileButton(*this);
}

void TileButton ::Initialize()
{
	GameObject* MenuObject = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("MenuController");

	if (MenuObject != nullptr)
	{
		menuController = MenuObject->GetComponent<MenuController>();
	}

	sprite = GetOwner()->GetComponent<Sprite>();
}

void TileButton::Clicked()
{
	if (menuController == nullptr)
	{
		Initialize();
	}

	menuController->SetSelectedTile(TileID);
}

void TileButton::SetTileID(int ID)
{
	TileID = ID;

	if (ID != 0)
	{
		sprite = GetOwner()->GetComponent<Sprite>();
		sprite->SetSpriteSource(Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("TileMap")->GetComponent<SpriteTilemap>()->GetSpriteSource());
		sprite->RefreshAutoMesh();
		sprite->SetFrame(ID - 1);
	}
}
