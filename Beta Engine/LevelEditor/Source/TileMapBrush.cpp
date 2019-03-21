#include "stdafx.h"
#include "TileMapBrush.h"
#include <Tilemap.h>
#include <CallbackInputManager.h>
#include <Input.h>
#include <Graphics.h>
#include <ColliderTilemap.h>
#include <GameObject.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <Parser.h>
#include <Transform.h>
#include <GameObject.h>
#include <Engine.h>
#include <SpaceManager.h>
#include "MenuController.h"

bool TileMapBrush::clicking = false;

TileMapBrush::TileMapBrush() : Component("TileMapBrush")
{
	map = nullptr;
	TileMapObject = nullptr;
	menuController = nullptr;
	menuObject = nullptr;
}

Component * TileMapBrush::Clone() const
{
	return new TileMapBrush(*this);
}

void TileMapBrush::Initialize()
{
	TileMapObject = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("TileMap");

	if (TileMapObject != nullptr)
	{
		map = TileMapObject->GetComponent<ColliderTilemap>()->GetTilemap();
	}

	menuObject = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("UI")->GetObjectManager().GetObjectByName("MenuController");

	if (menuObject != nullptr)
	{
		menuController = menuObject->GetComponent<MenuController>();
	}

	CallbackInputManager::GetInstance().addKeyPressBinding(GLFW_MOUSE_BUTTON_1, TileMapBrush::onMouseClick);
	CallbackInputManager::GetInstance().addKeyReleaseBinding(GLFW_MOUSE_BUTTON_1, TileMapBrush::onMouseClickUp);
}

void TileMapBrush::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (TileMapObject == nullptr || map == nullptr || menuController == nullptr || menuObject == nullptr)
	{
		Initialize();
		return;
	}

	if (menuController->GetEnabledTool() != ToolType::TMBrush) {
		return;
	}

	bool canBrush = !menuController->IsMouseOnUI();
	if (canBrush && !Input::GetInstance().IsKeyDown(VK_LBUTTON))
	{
		enabled = true;
	}
	else if (!canBrush) {
		enabled = false;
	}

	if (enabled && clicking) {
		PlaceTile(Graphics::GetInstance().ScreenToWorldPosition(Input::GetInstance().GetCursorPosition()));
	}
}

void TileMapBrush::SetTilemap(Tilemap * _map)
{
	map = _map;
}

void TileMapBrush::onMouseClick(int key)
{
	UNREFERENCED_PARAMETER(key);

	clicking = true;
}

void TileMapBrush::onMouseClickUp(int key)
{
	UNREFERENCED_PARAMETER(key);

	clicking = false;
}

void TileMapBrush::PlaceTile(Vector2D MousePos)
{

	ColliderTilemap* CT = TileMapObject->GetComponent<ColliderTilemap>();

	if (CT == nullptr)
	{
		Initialize();
		return;
	}

	Vector2D tile = CT->ConvertWorldCordsToTileMapCords(MousePos);

	int tileX = static_cast<int>(tile.x);
	int tileY = static_cast<int>(tile.y);

	//std::cout << "X : " << tileX << "Y : " << tileY << std::endl;

	Vector2D offset = map->SetCellValue(tileX, tileY, menuController->GetSelectedTile());
	offset.y = offset.y * CT->GetOwner()->GetComponent<Transform>()->GetScale().y;
	offset.x = offset.x * CT->GetOwner()->GetComponent<Transform>()->GetScale().x;

	Vector2D trans = CT->GetOwner()->GetComponent<Transform>()->GetTranslation();

	CT->GetOwner()->GetComponent<Transform>()->SetTranslation(trans + offset);
}
