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
#include <SpriteTilemap.h>
#include <Engine.h>
#include <SpaceManager.h>
#include <SpriteSource.h>
#include "TileButton.h"
#include "TileMapBrush.h"
#include "MenuController.h"
#include "GameObjectButton.h"

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
	UNREFERENCED_PARAMETER(parser);
}

void Menu::Deserialize(Parser & parser)
{
	UNREFERENCED_PARAMETER(parser);
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
	transform->SetScale(Vector2D(menuScale.x, (float)windowHeight));

	InitButtons();
}

void Menu::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
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

	//std::cout << GetOwner()->GetName() << ", " << inOnUI << std::endl;
    
    return inOnUI;
}

GameObject* Menu::InitTab(int order, int buffer)
{
    GameObject* newTab = GameObjectFactory::GetInstance().CreateObject("LevelEditor/Tab");
    newTab->GetComponent<Tab>()->SetMenu(GetOwner());
    SetTab(newTab);

    Sprite* tabSprite = newTab->GetComponent<Sprite>();
    Sprite* mySprite = GetOwner()->GetComponent<Sprite>();

    tabSprite->SetColor(mySprite->GetColor());

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
    for (size_t i = 0; i < buttons.size(); i++)
    {
		buttons[i]->GetComponent<Button>()->setEnabled(false);

        buttons[i]->GetComponent<Sprite>()->SetAlpha(0.0f);

		//std::cout << buttons[i]->GetName() << ", " << i << ", " << buttons[i]->GetComponent<Sprite>()->GetAlpha() << ", " << buttons[i]->GetComponent<Button>()->getEnabled() << std::endl;
    }
}

void Menu::ShowButtons()
{
    for (size_t i = 0; i < buttons.size(); i++)
    {	
		buttons[i]->GetComponent<Button>()->setEnabled(true);

        buttons[i]->GetComponent<Sprite>()->SetAlpha(1.0f);

		//std::cout << buttons[i]->GetName() << ", " << i << ", " << buttons[i]->GetComponent<Sprite>()->GetAlpha() << ", " << buttons[i]->GetComponent<Button>()->getEnabled() << std::endl;
    }
}

void Menu::InitButtons()
{
	if (buttons.size() > 0) {
		for (size_t i = 0; i < buttons.size(); i++)
		{
			buttons[i]->Destroy();
		}

		buttons.clear();
	}

	switch (menuType)
	{
	case TileMap:
	{
		GameObject* tileMapGO = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("TileMap");

		if (tileMapGO == nullptr)
		{
			return;
		}

		int TileCount = tileMapGO->GetComponent<SpriteTilemap>()->GetSpriteSource()->GetFrameCountTexture();

		int rows = 5;
		float xScale = 55;
		float yScale = 65;

		Transform* transform = GetOwner()->GetComponent<Transform>();

		for (int i = 0; i < TileCount; i++)
		{
			GameObject* button = GameObjectFactory::GetInstance().CreateObject("LevelEditor/TileButton");

			button->GetComponent<TileButton>()->SetTileID(i);

			Vector2D pos = Vector2D((float)(i % rows), (float)-(i / rows));

			Vector2D offset = Vector2D(-(rows * xScale / 2) + 30, (transform->GetScale().y / 2) - 75);

			button->GetComponent<Transform>()->SetTranslation(transform->GetTranslation() + Vector2D(pos.x * xScale, pos.y * yScale) + offset);

			GetOwner()->GetSpace()->GetObjectManager().AddObject(*button);

			buttons.push_back(button);
		}
		break;
	}
	case GameObjects:
	{
		GameObject* menuControllerGameObject = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("MenuController");

		if (menuControllerGameObject == nullptr)
		{
			return;
		}

		std::vector<std::string> gameObjectNames = menuControllerGameObject->GetComponent<MenuController>()->GetGameObjectNames();

		int rows = 3;
		float xScale = 100;
		float yScale = 80;

		Transform* transform = GetOwner()->GetComponent<Transform>();

		for (int i = 0; i < gameObjectNames.size(); i++)
		{
			GameObject* button = GameObjectFactory::GetInstance().CreateObject("LevelEditor/GOButton");
			
			button->GetComponent<GameObjectButton>()->SetGameObjectName(gameObjectNames[i]);

			Vector2D pos = Vector2D((float)(i % rows), (float)-(i / rows));

			Vector2D offset = Vector2D(-(rows * xScale / 2) + 40, (transform->GetScale().y / 2) - 75);

			button->GetComponent<Transform>()->SetTranslation(transform->GetTranslation() + Vector2D(pos.x * xScale, pos.y * yScale) + offset);

			GetOwner()->GetSpace()->GetObjectManager().AddObject(*button);

			buttons.push_back(button);
		}
		break;
	}
	default:
		break;
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
