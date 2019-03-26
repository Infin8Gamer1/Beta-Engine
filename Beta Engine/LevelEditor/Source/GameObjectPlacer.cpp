#include "stdafx.h"
#include "GameObjectPlacer.h"
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
#include <GameObjectFactory.h>
#include <CallbackInputManager.h>

bool GameObjectPlacer::clicked = false;

GameObjectPlacer::GameObjectPlacer() : Component("GameObjectPlacer")
{
	TileMapObject = nullptr;
	menuController = nullptr;
	menuObject = nullptr;
}

Component * GameObjectPlacer::Clone() const
{
	return new GameObjectPlacer(*this);
}

void GameObjectPlacer::Initialize()
{
	TileMapObject = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("TileMap");

	menuObject = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("UI")->GetObjectManager().GetObjectByName("MenuController");

	if (menuObject != nullptr)
	{
		menuController = menuObject->GetComponent<MenuController>();
	}

	CallbackInputManager::GetInstance().addKeyReleaseBinding(GLFW_MOUSE_BUTTON_1, GameObjectPlacer::onMouseClickUp);
}

void GameObjectPlacer::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (TileMapObject == nullptr || menuController == nullptr || menuObject == nullptr)
	{
		Initialize();
		return;
	}

	if (menuController->GetEnabledTool() != ToolType::GOPlacer) {
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

	//std::cout << enabled << std::endl;

	if (enabled && clicked) {
		PlaceObject(Graphics::GetInstance().ScreenToWorldPosition(Input::GetInstance().GetCursorPosition()));
	}
	else {
		clicked = false;
	}
}

void GameObjectPlacer::PlaceObject(Vector2D mousePosition)
{
	Vector2D GOPosition = mousePosition;

	ColliderTilemap* CT = TileMapObject->GetComponent<ColliderTilemap>();

	if (CT != nullptr)
	{
		GOPosition = CT->ConvertWorldCordsToTileMapCords(mousePosition);
		GOPosition = CT->ConvertTileMapCordsToWorldCords(GOPosition);
	}

	std::string GOName = menuController->GetSelectedGameObjectName();
	if (GOName == "")
	{
		return;
	}

	GameObject* object = GameObjectFactory::GetInstance().CreateObject(GOName);

	std::string newName = TileMapObject->GetSpace()->GetObjectManager().GenerateUniqueGameObjectName(object->GetName());

	object->SetName(newName);

	Transform* trans = object->GetComponent<Transform>();

	if (trans != nullptr)
	{
		trans->SetTranslation(GOPosition);
	}

	TileMapObject->GetSpace()->GetObjectManager().AddObject(*object);

	object->CreateTweakBar();

	clicked = false;
}

void GameObjectPlacer::onMouseClickUp(int key)
{
	UNREFERENCED_PARAMETER(key);

	clicked = true;
}
