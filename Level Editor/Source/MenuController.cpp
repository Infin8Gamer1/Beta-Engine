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

MenuController::MenuController() : Component("MenuController")
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

		menus.push_back(newMenu);
    }
}

void MenuController::Update(float dt)
{
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