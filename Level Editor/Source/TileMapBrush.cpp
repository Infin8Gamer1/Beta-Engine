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
#include <Transform.h>
#include <GameObject.h>

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
	if (enabled && Input::GetInstance().CheckTriggered(VK_LBUTTON)) {
		PlaceTile(Graphics::GetInstance().ScreenToWorldPosition(Input::GetInstance().GetCursorPosition()));
	}
}

void TileMapBrush::SetTilemap(Tilemap * _map)
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

void TileMapBrush::Enable()
{
    enabled = true;
}

void TileMapBrush::Disable()
{
    enabled = false;
}

void TileMapBrush::PlaceTile(Vector2D MousePos)
{
	ColliderTilemap* CT = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>();
	Vector2D tile = CT->ConvertWorldCordsToTileMapCords(MousePos);

	int tileX = static_cast<int>(tile.x);
	int tileY = static_cast<int>(tile.y);

	std::cout << "X : " << tileX << "Y : " << tileY << std::endl;

	Vector2D offset = map->SetCellValue(tileX, tileY, SelectedTileID);
	offset.y = offset.y * CT->GetOwner()->GetComponent<Transform>()->GetScale().y;
	offset.x = offset.x * CT->GetOwner()->GetComponent<Transform>()->GetScale().x;

	Vector2D trans = CT->GetOwner()->GetComponent<Transform>()->GetTranslation();

	CT->GetOwner()->GetComponent<Transform>()->SetTranslation(trans + offset);
}
