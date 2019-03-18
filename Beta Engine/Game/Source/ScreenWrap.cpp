//------------------------------------------------------------------------------
//
// File Name:	ScreenWrap.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ScreenWrap.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include <Graphics.h>

Behaviors::ScreenWrap::ScreenWrap() : Component("ScreenWrap")
{
}

Component * Behaviors::ScreenWrap::Clone() const
{
	return new ScreenWrap();
}

void Behaviors::ScreenWrap::Initialize()
{
	transform = GetOwner()->GetComponent<Transform>();
	physics = GetOwner()->GetComponent<Physics>();
}

void Behaviors::ScreenWrap::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	Vector2D currentTranslation = transform->GetTranslation();
	Vector2D currentScale = transform->GetScale();
	Vector2D currentVelocity = physics->GetVelocity();

	Vector2D newTranslation = currentTranslation;

	BoundingRectangle screen = Graphics::GetInstance().GetScreenWorldDimensions();

	//   moving right         and          outside right side of screen
	if (currentVelocity.x > 0 && currentTranslation.x > screen.right + currentScale.x/2) {
		newTranslation.x = screen.left - currentScale.x/2;
	}

	//   moving left          and          outside left side of screen
	if (currentVelocity.x < 0 && currentTranslation.x < screen.left - currentScale.x/2) {
		newTranslation.x = screen.right + currentScale.x/2;
	}

	//   moving up            and          outside top of screen
	if (currentVelocity.y > 0 && currentTranslation.y > screen.top + currentScale.y/2) {
		newTranslation.y = screen.bottom - currentScale.y/2;
	}

	//   moving down          and          outside bottom of screen
	if (currentVelocity.y < 0 && currentTranslation.y < screen.bottom - currentScale.y/2) {
		newTranslation.y = screen.top + currentScale.y/2;
	}

	transform->SetTranslation(newTranslation);

	/*//if the new translation is diffrent than the current one then set it to the current one
	if ((newTranslation.x != currentTranslation.x) || (newTranslation.y != currentTranslation.y)) {
		transform->SetTranslation(newTranslation);
	}*/
}
