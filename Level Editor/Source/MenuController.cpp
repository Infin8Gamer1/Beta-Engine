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

MenuController::MenuController() : Component("MenuController"), tabBuffer(50)
{
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
    for (int i = 0; i < menuCount; i++)
    {
        GameObject* newMenu = GameObjectFactory::GetInstance().CreateObject("Menu");
		newMenu->GetComponent<Menu>()->SetMenuController(GetOwner());

        GetOwner()->GetSpace()->GetObjectManager().AddObject(*newMenu);

        Vector2D menuScale = newMenu->GetComponent<Transform>()->GetScale();

        GameObject* newTab = GameObjectFactory::GetInstance().CreateObject("Tab");
        newMenu->GetComponent<Menu>()->SetTab(newTab);
        newTab->GetComponent<Tab>()->SetMenu(newMenu);

        Transform* tabTransform = newTab->GetComponent<Transform>();

        Vector2D TabPos = Vector2D((newMenu->GetComponent<Transform>()->GetTranslation().x - (menuScale.x / 2)) - (tabTransform->GetScale().x / 2), (menuScale.y / 2) - (tabTransform->GetScale().y) - (tabTransform->GetScale().y * i + tabBuffer));

        tabTransform->SetTranslation(TabPos);

        GetOwner()->GetSpace()->GetObjectManager().AddObject(*newTab);

		menus.push_back(newMenu);
    }

    brush = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("Brush")->GetComponent<TileMapBrush>();
}

void MenuController::Update(float dt)
{
    bool canBrush = true;

    for (size_t i = 0; i < menus.size(); i++)
    {
        if (menus[i]->GetComponent<Menu>()->IsMouseOnUI())
        {
            canBrush = false;
        }
        
    }

    if (canBrush)
    {
        brush->Enable();
    }
    else
    {
        brush->Disable();
    }
}

void MenuController::ToggleMenus()
{
    
}

void MenuController::ShowMenu(GameObject * menu)
{
    for (int i = 0; i < menuCount; i++)
    {
        if (&menu == &menus[i])
        {
            menus[i]->GetComponent<Sprite>()->SetAlpha(1.0f);
        }
        else
        {
            menus[i]->GetComponent<Sprite>()->SetAlpha(0.0f);
        }
        
    }
}