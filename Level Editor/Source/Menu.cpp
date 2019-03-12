#include "stdafx.h"
#include "Menu.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"

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
}

void Menu::Update(float dt)
{

}

Vector2D Menu::GetMousePosition()
{
    Vector2D mousepos = Input::GetInstance().GetCursorPosition();
    return Graphics::GetInstance().ScreenToWorldPosition(mousepos, &Graphics::GetInstance().GetCurrentCamera());
}

//dont forget to auctualy check if the left mouse has been clicked too
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
