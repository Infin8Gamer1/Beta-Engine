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
#include <Space.h>
#include <Parser.h>
#include <GameObjectFactory.h>

Level::Level(const std::string & name) : BetaObject(name)
{
	fileLocation = "";
}

Space * Level::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}

void Level::Deserialize(Parser & parser)
{
	parser.ReadSkip(GetName());
	parser.ReadSkip("{");

	/*std::string TileMapName;
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
	}*/

	unsigned numGameObjects = 0;
	parser.ReadVar(numGameObjects);

	parser.ReadSkip("{");

	for (unsigned i = 0; i < numGameObjects; i++)
	{
		std::string GameObjectName;
		parser.ReadVar(GameObjectName);

		GameObject* object = GameObjectFactory::GetInstance().CreateObject(GameObjectName);

		GetSpace()->GetObjectManager().AddObject(*object);
	}

	parser.ReadSkip("}");

	parser.ReadSkip("}");
}

void Level::Serialize(Parser & parser) const
{
	parser.WriteValue(GetName());

	parser.BeginScope();

	/*GameObject* tileMapGO = GetSpace()->GetObjectManager().GetObjectByName("TileMap");

	if (tileMapGO != nullptr) {
		SpriteTilemap* spriteTileMap = tileMapGO->GetComponent<SpriteTilemap>();

		Tilemap* map = spriteTileMap->GetTilemap();
		SpriteSource* ss = spriteTileMap->GetSpriteSource();
		Transform* transform = spriteTileMap->GetOwner()->GetComponent<Transform>();

		//save the map to it's file
		ResourceManager::GetInstance().SaveTilemapToFile(map);

		//save the location of the map
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
	}*/

	std::vector<GameObject*> activeGOs = GetSpace()->GetObjectManager().GetGameObjectActiveList();

	unsigned numGameObjects = (unsigned)activeGOs.size();
	parser.WriteVar(numGameObjects);

	parser.BeginScope();

	for (unsigned i = 0; i < numGameObjects; i++)
	{
		std::string GameObjectName = activeGOs[i]->GetName();
		parser.WriteVar(GameObjectName);

		GameObjectFactory::GetInstance().SaveObjectToFile(activeGOs[i]);
	}

	parser.EndScope();

	parser.EndScope();
}

void Level::SaveLevel()
{
	if (GetFileLocation() != "") {
		Parser* parser = new Parser(GetFileLocation(), std::fstream::out);

		try
		{
			Serialize(*parser);
		}
		catch (ParseException e)
		{
			std::cout << e.what() << std::endl;

			DisplayMessage(e.what());
		}

		delete parser;
		parser = nullptr;
	}
}

void Level::LoadLevel()
{
	if (GetFileLocation() != "") {
		Parser* parser = new Parser(GetFileLocation(), std::fstream::in);

		try
		{
			Deserialize(*parser);
		}
		catch (ParseException e)
		{
			std::cout << e.what() << std::endl;

			DisplayMessage(e.what());
		}

		delete parser;
		parser = nullptr;
	}
}

void Level::SetFileLocation(std::string _fileLocation)
{
	fileLocation = _fileLocation;
}

std::string Level::GetFileLocation()
{
	return fileLocation;
}

void Level::DisplayMessage(std::string what)
{
	MessageBox(
		NULL,
		what.c_str(),
		"There Was An ERROR!",
		MB_ICONHAND | MB_OK | MB_APPLMODAL
	);
}


