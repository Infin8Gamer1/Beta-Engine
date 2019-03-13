#include "stdafx.h"
#include "TileButton.h"
#include <Sprite.h>
#include <Engine.h>
#include <Space.h>
#include <SpaceManager.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <SpriteTilemap.h>
#include "TileMapBrush.h"

TileButton::TileButton() : Button("TileButton")
{
}

Component * TileButton::Clone() const
{
	return new TileButton(*this);
}

void TileButton::Clicked()
{
	brush->SetSelectedTile(TileID);
}

void TileButton::SetBrush(TileMapBrush* _brush)
{
	brush = _brush;
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
