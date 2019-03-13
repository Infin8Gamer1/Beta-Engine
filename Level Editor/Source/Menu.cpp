#include "stdafx.h"
#include "Menu.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include <GameObject.h>
#include "Transform.h"
#include "Physics.h"
#include "GameObjectFactory.h"
#include "Space.h"
#include "Tab.h"
#include <System.h>
#include <glfw3.h>
#include <SpriteTilemap.h>
#include <Engine.h>
#include <SpaceManager.h>
#include <SpriteSource.h>
#include "TileButton.h"
#include "TileMapBrush.h"

Menu::Menu() : Component("Menu")
{
	tab = nullptr;
}

Component * Menu::Clone() const
{
	return new Menu(*this);
}

void Menu::Serialize(Parser & parser) const
{
}

void Menu::Deserialize(Parser & parser)
{
}

void Menu::Initialize()
{
	Transform* transform = GetOwner()->GetComponent<Transform>();

	Vector2D menuScale = transform->GetScale();

	int windowWidth, windowHeight;
	GLFWwindow* handle = System::GetInstance().GetWindowHandle();
	glfwGetWindowSize(handle, &windowWidth, &windowHeight);

	Vector2D menuLocation = Vector2D((windowWidth / 2) - (menuScale.x / 2), 0);

	transform->SetTranslation(menuLocation);
	transform->SetScale(Vector2D(menuScale.x, windowHeight));

	menuScale = transform->GetScale();

    GameObject* newTab = GameObjectFactory::GetInstance().CreateObject("Tab");
    SetTab(newTab);
    tab->GetComponent<Tab>()->SetMenu(GetOwner());

	Transform* tabTransform = tab->GetComponent<Transform>();

    Vector2D TabPos = Vector2D((transform->GetTranslation().x - (menuScale.x / 2)) - (tabTransform->GetScale().x / 2), (menuScale.y / 2) - (tabTransform->GetScale().y));

    tabTransform->SetTranslation(TabPos);

    GetOwner()->GetSpace()->GetObjectManager().AddObject(*newTab);

	InitButtons(MenuType::TileMap);
}

void Menu::Update(float dt)
{
}

void Menu::SetTab(GameObject* tab_)
{
    tab = tab_;
}

GameObject* Menu::GetTab()
{
    return tab;
}

GameObject * Menu::GetMenuController()
{
    return menuController;
}

void Menu::SetMenuController(GameObject * controller_)
{
    menuController = controller_;
}

bool Menu::IsShown()
{
    return isShown;
}

void Menu::setIsShown(bool show)
{
	isShown = show;
}

void Menu::InitButtons(MenuType type)
{
	int TileCount = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("TileMap")->GetComponent<SpriteTilemap>()->GetSpriteSource()->GetFrameCountTexture();

	int rows = 3;

	Transform* transform = GetOwner()->GetComponent<Transform>();

	for (int i = 0; i < TileCount; i++)
	{
		GameObject* button = GameObjectFactory::GetInstance().CreateObject("TileButton");

		button->GetComponent<TileButton>()->SetTileID(i);
		
		button->GetComponent<TileButton>()->SetBrush(Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("Brush")->GetComponent<TileMapBrush>());
		
		Vector2D pos = Vector2D(i / rows, i % rows);

		Vector2D offset = Vector2D(-100, 0);

		button->GetComponent<Transform>()->SetTranslation(transform->GetTranslation() + (pos * 100) + offset);

		GetOwner()->GetSpace()->GetObjectManager().AddObject(*button);
	}
}
