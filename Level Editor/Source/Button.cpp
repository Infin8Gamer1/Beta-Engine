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
}

void Button::Initialize()
{
}

void Button::Update(float dt)
{
    if (IsClicked() && Input::GetInstance().CheckTriggered(VK_LBUTTON))
    {
        Clicked();
    }
}

Vector2D Button::GetMousePosition()
{
    Vector2D mousepos = Input::GetInstance().GetCursorPosition();
    return Graphics::GetInstance().ScreenToWorldPosition(mousepos);
}

bool Button::IsClicked()
{
	TileMapBrush* brush = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetObjectManager().GetObjectByName("Brush")->GetComponent<TileMapBrush>();

    Vector2D pos = GetMousePosition();
    Transform* tran = GetOwner()->GetComponent<Transform>();

    BoundingRectangle rect = BoundingRectangle(tran->GetTranslation(), tran->GetScale() / 2.0f);
    if (PointRectangleIntersection(pos, rect))
    {
        brush->Disable();
        return true;
    }

    brush->Enable();
    return false;
}