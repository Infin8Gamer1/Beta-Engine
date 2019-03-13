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

	InitButtons(menuType);
}

void Menu::Update(float dt)
{
}

bool Menu::IsMouseOnUI()
{
    bool inOnUI = false;

    for (size_t i = 0; i < buttons.size(); i++)
    {
        if (buttons[i]->GetComponent<Button>()->getIsHovered())
        {
            inOnUI = true;
        }
    }

    if (tab->GetComponent<Button>()->getIsHovered())
    {
        inOnUI = true;
    }
    
    return inOnUI;
}

GameObject* Menu::InitTab(int order, int buffer)
{
    GameObject* newTab = GameObjectFactory::GetInstance().CreateObject("Tab");
    newTab->GetComponent<Tab>()->SetMenu(GetOwner());
    SetTab(newTab);

    Transform* tabTransform = newTab->GetComponent<Transform>();

    Vector2D menuScale = GetOwner()->GetComponent<Transform>()->GetScale();

    Vector2D TabPos = Vector2D((GetOwner()->GetComponent<Transform>()->GetTranslation().x - (menuScale.x / 2)) - (tabTransform->GetScale().x / 2), (menuScale.y / 2) - (tabTransform->GetScale().y) - (tabTransform->GetScale().y * order + buffer * order));

    tabTransform->SetTranslation(TabPos);

    GetOwner()->GetSpace()->GetObjectManager().AddObject(*newTab);
    return newTab;
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

void Menu::HideButtons()
{
    std::cout << "Showing... " + GetOwner()->GetName() + " " << menuType << std::endl;
    for (unsigned i = 0; i < buttons.size(); ++i)
    {
        std::cout << buttons[i]->GetName() << std::endl;

        if (buttons[i] == tab) continue;
        if(menuType == TileMap) buttons[i]->GetComponent<TileButton>()->setEnabled(false);
        buttons[i]->GetComponent<Sprite>()->SetAlpha(0.0f);
    }
}

void Menu::ShowButtons()
{
    std::cout << "Hiding... " + GetOwner()->GetName() + " " << menuType << std::endl;

    for (unsigned i = 0; i < buttons.size(); ++i)
    {
        std::cout << buttons[i]->GetName() << std::endl;
        if (buttons[i] == tab) continue;
        if (menuType == TileMap) buttons[i]->GetComponent<TileButton>()->setEnabled(true);
        buttons[i]->GetComponent<Sprite>()->SetAlpha(1.0f);
    }
}



void Menu::InitButtons(MenuType type)
{
    if (type != TileMap) return;

	int TileCount = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("TileMap")->GetComponent<SpriteTilemap>()->GetSpriteSource()->GetFrameCountTexture();

	int rows = 5;
	float xScale = 55;
	float yScale = 65;

	Transform* transform = GetOwner()->GetComponent<Transform>();

	for (int i = 0; i < TileCount; i++)
	{
		GameObject* button = GameObjectFactory::GetInstance().CreateObject("TileButton");

		button->GetComponent<TileButton>()->SetTileID(i);
		
		button->GetComponent<TileButton>()->SetBrush(Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("Brush")->GetComponent<TileMapBrush>());
		
		Vector2D pos = Vector2D(i % rows, -(i / rows));

		Vector2D offset = Vector2D(-(rows * xScale / 2) + 30, 300);

		button->GetComponent<Transform>()->SetTranslation(transform->GetTranslation() + Vector2D(pos.x * xScale, pos.y * yScale) + offset);

		GetOwner()->GetSpace()->GetObjectManager().AddObject(*button);

		buttons.push_back(button);
	}
}

void Menu::SetType(MenuType type)
{
    menuType = type;
}

MenuType Menu::GetType()
{
    return menuType;
}
