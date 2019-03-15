//------------------------------------------------------------------------------
//
// File Name:	Level.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Level.h"
#include "Space.h"
#include <Parser.h>
#include <Tilemap.h>
#include <ResourceManager.h>
#include <GameObjectFactory.h>
#include <GameObject.h>
#include <Transform.h>
#include <SpriteTilemap.h>
#include <ColliderTilemap.h>
#include <SpriteSource.h>

Level::Level(const std::string & name) : BetaObject(name)
{

}

Space * Level::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}

void Level::Deserialize(Parser & parser)
{
	parser.ReadSkip(GetName());
	parser.ReadSkip("{");
	
	std::string TileMapName;
	parser.ReadVar(TileMapName);

	if (TileMapName != "Null" && TileMapName != "")
	{
		Tilemap* map = ResourceManager::GetInstance().GetTilemap(TileMapName, true, true);

		if (map == nullptr) {
			throw ParseException(TileMapName, "TileMap " + TileMapName + " could not be found! ERROR 404");
		}

		Vector2D TileMapPos;
		parser.ReadVar(TileMapPos);

		Vector2D TileMapScale;
		parser.ReadVar(TileMapScale);

		std::string TileMapSpriteSource;
		parser.ReadVar(TileMapSpriteSource);

		SpriteSource* ss = ResourceManager::GetInstance().GetSpriteSource(TileMapSpriteSource, true);

		GameObject* otherTileMap = GetSpace()->GetObjectManager().GetObjectByName("TileMap");

		if (otherTileMap != nullptr)
		{
			otherTileMap->Destroy();
		}

		GameObject* GO = GameObjectFactory::GetInstance().CreateObject("TileMap");

		GO->GetComponent<Transform>()->SetScale(TileMapScale);
		GO->GetComponent<Transform>()->SetTranslation(TileMapPos);

		GO->GetComponent<SpriteTilemap>()->SetTilemap(map);
		GO->GetComponent<SpriteTilemap>()->SetSpriteSource(ss);

		GO->GetComponent<ColliderTilemap>()->SetTilemap(map);

		GetSpace()->GetObjectManager().AddObject(*GO);
		//gameObjects.push_back(GO);
	}

	unsigned numGameObjects = 0;
	parser.ReadVar(numGameObjects);

	parser.ReadSkip("{");

	for (unsigned i = 0; i < numGameObjects; i++)
	{
		std::string GameObjectName;
		parser.ReadVar(GameObjectName);

		GameObject* object = GameObjectFactory::GetInstance().CreateObject(GameObjectName);

		GetSpace()->GetObjectManager().AddObject(*object);
		gameObjects.push_back(object);
	}

	parser.ReadSkip("}");

	parser.ReadSkip("}");
}

void Level::Serialize(Parser & parser) const
{
	parser.WriteValue(GetName());

	parser.BeginScope();

	GameObject* tileMapGO = GetSpace()->GetObjectManager().GetObjectByName("TileMap");

	if (tileMapGO != nullptr) {
		SpriteTilemap* spriteTileMap = tileMapGO->GetComponent<SpriteTilemap>();

		Tilemap* map = spriteTileMap->GetTilemap();
		ResourceManager::GetInstance().SaveTilemapToFile(map);

		SpriteSource* ss = spriteTileMap->GetSpriteSource();
		Transform* transform = spriteTileMap->GetOwner()->GetComponent<Transform>();

		std::string TileMapName = map->GetName();
		parser.WriteVar(TileMapName);

		Vector2D TileMapPos = transform->GetTranslation();
		parser.WriteVar(TileMapPos);

		Vector2D TileMapScale = transform->GetScale();
		parser.WriteVar(TileMapScale);

		std::string TileMapSpriteSource = ss->GetName();
		parser.WriteVar(TileMapSpriteSource);

		
	}
	else {
		std::string TileMapName = "Null";
		parser.WriteVar(TileMapName);
	}

	unsigned numGameObjects = gameObjects.size();
	parser.WriteVar(numGameObjects);

	parser.BeginScope();

	for (unsigned i = 0; i < numGameObjects; i++)
	{
		std::string GameObjectName = gameObjects[i]->GetName();
		parser.WriteVar(GameObjectName);
	}

	parser.EndScope();

	parser.EndScope();
}
