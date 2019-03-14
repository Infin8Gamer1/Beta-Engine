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

	unsigned numTileMaps = 0;
	parser.ReadVar(numTileMaps);

	for (unsigned i = 0; i < numTileMaps; i++)
	{
		std::string TileMapName;
		parser.ReadValue(TileMapName);

		Tilemap* map  = ResourceManager::GetInstance().GetTilemap(TileMapName, true, true);

		if (map == nullptr) {
			throw ParseException(TileMapName, "TileMap " + TileMapName + " could not be found! ERROR 404");
		}
	}

	unsigned numGameObjects = 0;
	parser.ReadVar(numGameObjects);

	for (unsigned i = 0; i < numGameObjects; i++)
	{
		std::string GameObjectName;
		parser.ReadValue(GameObjectName);

		//Tilemap* map = ResourceManager::GetInstance().GetTilemap(GameObjectName);//GameObjectFactory::GetInstance().CreateComponent(componentName);

		//if (map == nullptr) {
		//	throw ParseException(TileMapName, "TileMap " + TileMapName + " could not be found! ERROR 404");
		//}
	}

	parser.ReadSkip("}");
}

void Level::Serialize(Parser & parser) const
{
}
