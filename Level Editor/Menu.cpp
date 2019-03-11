#include "Menu.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"

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

bool Menu::IsTabClickedOn()
{
    Vector2D pos = GetMousePosition();
    Transform* tran = static_cast<Transform*>(tab.GetComponent("Transform"));

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
