#include "stdafx.h"
#include "SaveManager.h"
#include <GameObject.h>
#include <Space.h>
#include <ColliderTilemap.h>
#include <GameObjectManager.h>
#include <Tilemap.h>


Behaviors::SaveManager::SaveManager() : Component("SaveManager")
{
}

Component * Behaviors::SaveManager::Clone() const
{
	return new SaveManager(*this);
}

void Behaviors::SaveManager::Initialize()
{
	map = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>()->GetTilemap();
}

void Behaviors::SaveManager::Update(float dt)
{
}

void Behaviors::SaveManager::Load()
{
	/*Parser* parser = new Parser(TileMapsFilePath + tilemapName + ".tileMap", std::fstream::in);

	map->Serialize(*parser);*/
}

void Behaviors::SaveManager::Save()
{
}