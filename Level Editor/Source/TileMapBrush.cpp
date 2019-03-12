#include "stdafx.h"
#include "TileMapBrush.h"
#include <Tilemap.h>
#include <Input.h>
#include <Graphics.h>
#include <ColliderTilemap.h>
#include <GameObject.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <Parser.h>

TileMapBrush::TileMapBrush() : Component("TileMapBrush")
{
}

Component * TileMapBrush::Clone() const
{
	return new TileMapBrush(*this);
}

void TileMapBrush::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	//see if the player has clicked and has enough tiles
	if (Input::GetInstance().CheckTriggered(VK_LBUTTON)) {
		PlaceTile(Graphics::GetInstance().ScreenToWorldPosition(Input::GetInstance().GetCursorPosition()));
	}
}

void TileMapBrush::SetTilemap(const Tilemap * _map)
{
	map = _map;
}

void TileMapBrush::SetSelectedTile(int selectedTileID)
{
	SelectedTileID = selectedTileID;
}

void TileMapBrush::Serialize(Parser & parser) const
{
	parser.WriteVariable("SelectedTileID", SelectedTileID);
}

void TileMapBrush::Deserialize(Parser & parser)
{
	parser.ReadVariable("SelectedTileID", SelectedTileID);
}

void TileMapBrush::PlaceTile(Vector2D MousePos)
{
	ColliderTilemap* CT = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();
	Vector2D tile = CT->ConvertWorldCordsToTileMapCords(MousePos);

	int tileX = static_cast<int>(tile.x);
	int tileY = static_cast<int>(tile.y);

	std::cout << "X : " << tileX << "Y : " << tileY << std::endl;

	//if the chosen path tile is already equal to selected tile then return
	if (map->GetCellValue(tileX, tileY) == SelectedTileID) {
		return;
	}

	map->SetCellValue(tileX, tileY, SelectedTileID);
}
