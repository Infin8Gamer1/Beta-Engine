#include "stdafx.h"

#include "MenuController.h"
#include "Menu.h"
#include "Sprite.h" 
#include "Button.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include "Transform.h"
#include "Physics.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Space.h"
#include "Parser.h"
#include "Tab.h"
#include "TileMapBrush.h"
#include <Engine.h>
#include <SpaceManager.h>
#include "glfw3.h"
#include <System.h>

MenuController::MenuController() : Component("MenuController"), tabBuffer(10)
{
	SelectedTileID = 0;
	SelectedGameObjectTemplateName = "";
}

Component * MenuController::Clone() const
{
    return new MenuController(*this);
}

void MenuController::Serialize(Parser & parser) const
{
    parser.WriteVariable("menuCount", menuCount);
}

void MenuController::Deserialize(Parser & parser)
{
    parser.ReadVariable("menuCount", menuCount);
}

void MenuController::Initialize()
{
    GameObject* TileMenu = GameObjectFactory::GetInstance().CreateObject("TileMenu");
    TileMenu->GetComponent<Menu>()->SetMenuController(GetOwner());
    TileMenu->GetComponent<Menu>()->SetType(TileMap);
    GetOwner()->GetSpace()->GetObjectManager().AddObject(*TileMenu);
    menus.push_back(TileMenu);
    GameObject* TileTab = TileMenu->GetComponent<Menu>()->InitTab(0, tabBuffer);

    GameObject* ObjectMenu = GameObjectFactory::GetInstance().CreateObject("ObjectMenu");
    ObjectMenu->GetComponent<Menu>()->SetMenuController(GetOwner());
    ObjectMenu->GetComponent<Menu>()->SetType(GameObjects);
    GetOwner()->GetSpace()->GetObjectManager().AddObject(*ObjectMenu);
    menus.push_back(ObjectMenu);
    GameObject* ObjectTab = ObjectMenu->GetComponent<Menu>()->InitTab(1, tabBuffer);

    ShowMenu(TileMenu);
}

bool MenuController::IsMouseOnUI()
{
	bool IsMouseOnUI = false;

	for (size_t i = 0; i < menus.size(); i++)
	{
		if (menus[i]->GetComponent<Menu>()->IsMouseOnUI())
		{
			IsMouseOnUI = true;
		}

	}
	for (size_t i = 0; i < buttons.size(); i++)
	{
		if (buttons[i]->getIsHovered())
		{
			IsMouseOnUI = true;
		}
	}

	return IsMouseOnUI;
}

void MenuController::ShowMenu(GameObject * menu)
{
    for (int i = 0; i < menuCount; i++)
    {
        if (menu == menus[i])
        {
            menus[i]->GetComponent<Sprite>()->SetAlpha(1.0f);
            menus[i]->GetComponent<Menu>()->setIsShown(true);
            menus[i]->GetComponent<Menu>()->ShowButtons();
        }
        else
        {
            menus[i]->GetComponent<Sprite>()->SetAlpha(0.0f);
            menus[i]->GetComponent<Menu>()->setIsShown(false);
            menus[i]->GetComponent<Menu>()->HideButtons();
        }
    }
}

void MenuController::ShiftTabsPos()
{
    int windowWidth, windowHeight;
    GLFWwindow* handle = System::GetInstance().GetWindowHandle();
    glfwGetWindowSize(handle, &windowWidth, &windowHeight);

    for (int i = 0; i < menuCount; i++)
    {
        Transform* tran = menus[i]->GetComponent<Menu>()->GetTab()->GetComponent<Transform>();

        tran->SetTranslation(Vector2D((windowWidth / 2) - (tran->GetScale().x / 2), tran->GetTranslation().y));
    }
}

void MenuController::RestoreTabsPos()
{
    int windowWidth, windowHeight;
    GLFWwindow* handle = System::GetInstance().GetWindowHandle();
    glfwGetWindowSize(handle, &windowWidth, &windowHeight);

    for (int i = 0; i < menuCount; i++)
    {
        Transform* tran = menus[i]->GetComponent<Menu>()->GetTab()->GetComponent<Transform>();

        float x = (menus[i]->GetComponent<Transform>()->GetTranslation().x - (menus[i]->GetComponent<Transform>()->GetScale().x / 2)) - (tran->GetScale().x / 2);

        tran->SetTranslation(Vector2D(x, tran->GetTranslation().y));
    }
}

int MenuController::GetSelectedTile()
{
	return SelectedTileID;
}

void MenuController::SetSelectedTile(int ID)
{
	SelectedTileID = ID;
}

std::string MenuController::GetSelectedGameObjectName()
{
	return SelectedGameObjectTemplateName;
}

void MenuController::SetSelectedGameObjectName(std::string name)
{
	SelectedGameObjectTemplateName = name;
}

void MenuController::AddGameObjectNames(std::string name)
{
	GameObjectNames.push_back(name);

	menus[1]->GetComponent<Menu>()->InitButtons();
}

std::vector<std::string> MenuController::GetGameObjectNames()
{
	return GameObjectNames;
}
