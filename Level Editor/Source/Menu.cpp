#include "stdafx.h"
#include "Menu.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Space.h"

Menu::Menu() : Component("Menu")
{
	tab = nullptr;
}

Component * Menu::Clone() const
{
	return new Menu();
}

void Menu::Serialize(Parser & parser) const
{
}

void Menu::Deserialize(Parser & parser)
{
}

void Menu::Initialize()
{
    GameObject* Tab = GameObjectFactory::GetInstance().CreateObject("Tab");
    SetTab(Tab);

    GetOwner()->GetSpace()->GetObjectManager().AddObject(*Tab);
}

void Menu::Update(float dt)
{
    if (IsTabClickedOn())
    {
        timer += dt;
        ToggleTab();
    }
}

Vector2D Menu::GetMousePosition()
{
    Vector2D mousepos = Input::GetInstance().GetCursorPosition();
    return Graphics::GetInstance().ScreenToWorldPosition(mousepos, &Graphics::GetInstance().GetCurrentCamera());
}

//dont forget to actualy check if the left mouse has been clicked too //DURRRRR, forgot .-.
bool Menu::IsTabClickedOn()
{
    Vector2D pos = GetMousePosition();
    Transform* tran = tab->GetComponent<Transform>();

    BoundingRectangle rect = BoundingRectangle(tran->GetTranslation(), tran->GetScale() / 2.0f);
    if (PointRectangleIntersection(pos, rect))
    {
        return true;
    }

    return false;
}

void Menu::ToggleTab()
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
