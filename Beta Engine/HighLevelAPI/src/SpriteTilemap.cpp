//------------------------------------------------------------------------------
//
// File Name:	SpriteTilemap.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteTilemap.h"
#include "Tilemap.h"
#include "Transform.h"
#include <Sprite.h>
#include <vector>
#include <Parser.h>
#include <ResourceManager.h>

SpriteTilemap::SpriteTilemap() : Sprite()
{
	map = nullptr;
}

Component * SpriteTilemap::Clone() const
{
	return new SpriteTilemap(*this);
}

void SpriteTilemap::Deserialize(Parser & parser)
{
	Sprite::Deserialize(parser);

	std::string tilemapLoc;
	parser.ReadVariable("tilemapLoc", tilemapLoc);

	SetTilemap(ResourceManager::GetInstance().GetTilemap(tilemapLoc, true, true));
}

void SpriteTilemap::Serialize(Parser & parser) const
{
	Sprite::Serialize(parser);

	parser.WriteVariable("tilemapLoc", map->GetName());

	ResourceManager::GetInstance().SaveTilemapToFile(map);
}

void SpriteTilemap::Draw()
{
	int height = map->GetHeight();
	int width = map->GetWidth();
	Vector2D tileScale = transform->GetScale();

	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			int cellValue = map->GetCellValue(c, r);

			//skip if this cell is empty or has an error aka it is less than or equal to 0
			if(cellValue > 0)
			{
				SetFrame(cellValue - 1);

				Sprite::Draw(Vector2D(tileScale.x * c, tileScale.y * -r));
			}
		}
	}

}

void SpriteTilemap::SetTilemap(Tilemap * _map)
{
	map = _map;
}

Tilemap * SpriteTilemap::GetTilemap()
{
	return map;
}
