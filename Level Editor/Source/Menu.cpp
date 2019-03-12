#include "stdafx.h"
#include "Menu.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "GameObjectFactory.h"
#include "Space.h"
#include "Tab.h"

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
    GameObject* newTab = GameObjectFactory::GetInstance().CreateObject("Tab");
    SetTab(newTab);
    tab->GetComponent<Tab>()->SetMenu(GetOwner());

    Vector2D pos = Vector2D(GetOwner()->GetComponent<Transform>()->GetTranslation().x - GetOwner()->GetComponent<Transform>()->GetTranslation().x / 2.0f, 0.0f);

    tab->GetComponent<Transform>()->SetTranslation(pos);

    GetOwner()->GetSpace()->GetObjectManager().AddObject(*newTab);
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
