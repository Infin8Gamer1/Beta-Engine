#include "stdafx.h"
#include "Button.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Input.h"
#include "Intersection2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "Space.h"
#include <Engine.h>
#include "SpaceManager.h"
#include "TileMapBrush.h"

Button::Button(std::string name) : Component(name)
{
	enabled = true;
	isHovered = false;
}

void Button::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

    if (!enabled) return;

    if (IsMouseHovering() && Input::GetInstance().CheckTriggered(VK_LBUTTON))
    {
        Clicked();
    }
}

bool Button::getIsHovered()
{
	return isHovered;
}

void Button::setEnabled(bool value)
{
    enabled = value;
}

bool Button::getEnabled()
{
    return enabled;
}

Vector2D Button::GetMousePosition()
{
    Vector2D mousepos = Input::GetInstance().GetCursorPosition();
    return Graphics::GetInstance().ScreenToWorldPosition(mousepos);
}

bool Button::IsMouseHovering()
{
    Vector2D pos = GetMousePosition();
    Transform* tran = GetOwner()->GetComponent<Transform>();

    BoundingRectangle rect = BoundingRectangle(tran->GetTranslation(), tran->GetScale() / 2.0f);
    if (PointRectangleIntersection(pos, rect))
    {
        //brush->Disable();
		isHovered = true;
		//std::cout << "Colliding with button " << GetName() << std::endl;
        return true;
    }

    //brush->Enable();
	isHovered = false;
    return false;
}
