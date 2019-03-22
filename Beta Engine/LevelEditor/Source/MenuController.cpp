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
#include <System.h>

MenuController::MenuController() : Component("MenuController"), tabBuffer(10)
{
	SelectedTileID = 0;
	SelectedGameObjectTemplateName = "";
	EnabledTool = ToolType::TMBrush;
}

MenuController::~MenuController()
{
	TwRemoveVar(Engine::GetInstance().GetModule<SpaceManager>()->GetTwBar(), "Selected Tile ID");
	TwRemoveVar(Engine::GetInstance().GetModule<SpaceManager>()->GetTwBar(), "Selected GO Name");
	TwRemoveVar(Engine::GetInstance().GetModule<SpaceManager>()->GetTwBar(), "Selected Tool");
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
    GameObject* TileMenu = GameObjectFactory::GetInstance().CreateObject("LevelEditor/TileMenu");
    TileMenu->GetComponent<Menu>()->SetMenuController(GetOwner());
    TileMenu->GetComponent<Menu>()->SetType(TileMap);
    GetOwner()->GetSpace()->GetObjectManager().AddObject(*TileMenu);
    menus.push_back(TileMenu);
    TileMenu->GetComponent<Menu>()->InitTab(0, tabBuffer);

    GameObject* ObjectMenu = GameObjectFactory::GetInstance().CreateObject("LevelEditor/ObjectMenu");
    ObjectMenu->GetComponent<Menu>()->SetMenuController(GetOwner());
    ObjectMenu->GetComponent<Menu>()->SetType(GameObjects);
    GetOwner()->GetSpace()->GetObjectManager().AddObject(*ObjectMenu);
    menus.push_back(ObjectMenu);
    ObjectMenu->GetComponent<Menu>()->InitTab(1, tabBuffer);

    ShowMenu(TileMenu);

	TwEnumVal ToolTypesEV[] = { {ToolType::TMBrush, "Tile Map Brush"}, {ToolType::GOPlacer, "Game Object Placer"}, {ToolType::GOSelecter, "Game Object Selector"} };
	TW_TYPE_TOOL_TYPE = TwDefineEnum("ToolType", ToolTypesEV, 3);

	TwAddVarRW(Engine::GetInstance().GetModule<SpaceManager>()->GetTwBar(), "Selected Tool", TW_TYPE_TOOL_TYPE, &EnabledTool, " enum='0 {Tile Map Brush}, 1 {Game Object Placer}, 2 {Game Object Selector}' ");

	TwAddVarRO(Engine::GetInstance().GetModule<SpaceManager>()->GetTwBar(), "Selected Tile ID", TW_TYPE_INT8, &SelectedTileID, " label='Selected Tile ID' help='The Tile that is selected' ");
	TwAddVarRO(Engine::GetInstance().GetModule<SpaceManager>()->GetTwBar(), "Selected GO Name", TW_TYPE_STDSTRING, &SelectedGameObjectTemplateName, " label='Selected GO' help='The Game Object that is selected' ");
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
    for (size_t i = 0; i < menus.size(); i++)
    {
        if (menu == menus[i])
        {
            menus[i]->GetComponent<Sprite>()->SetAlpha(1.0f);
            menus[i]->GetComponent<Menu>()->setIsShown(true);
            menus[i]->GetComponent<Menu>()->ShowButtons();

			MenuType type = menus[i]->GetComponent<Menu>()->GetType();
			
			switch (type)
			{
			case TileMap:
				SetEnabledTool(ToolType::TMBrush);
				break;
			case GameObjects:
				SetEnabledTool(ToolType::GOPlacer);
				break;
			default:
				break;
			}
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

void MenuController::SetSelectedGameObjectName(std::string _name)
{
	SelectedGameObjectTemplateName = _name;
}

void MenuController::AddGameObjectNames(std::string _name)
{
	GameObjectNames.push_back(_name);

	menus[1]->GetComponent<Menu>()->InitButtons();

	ShowMenu(menus[1]);
	RestoreTabsPos();

	SetSelectedGameObjectName(_name);
}

std::vector<std::string> MenuController::GetGameObjectNames()
{
	return GameObjectNames;
}

ToolType MenuController::GetEnabledTool()
{
	return EnabledTool;
}

void MenuController::SetEnabledTool(ToolType tool)
{
	EnabledTool = tool;
}
