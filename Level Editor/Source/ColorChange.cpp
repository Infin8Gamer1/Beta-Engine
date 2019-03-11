//------------------------------------------------------------------------------
//
// File Name:	ColorChange.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColorChange.h"
#include "Sprite.h"
#include "Collider.h"
#include "GameObject.h"
#include <Parser.h>

Behaviors::ColorChange::ColorChange(const Color & _normalColor, const Color & _collidedColor, float _collidedColorTime) : Component("ColorChange")
{
	// Components
	sprite = nullptr;

	// Properties
	normalColor = _normalColor;
	collidedColor = _collidedColor;
	collidedColorTime = _collidedColorTime;

	// Other variables
	collided = false;
	timer = 0.0f;
}

Component * Behaviors::ColorChange::Clone() const
{
	return new ColorChange(normalColor, collidedColor, collidedColorTime);
}

void Behaviors::ColorChange::Deserialize(Parser & parser)
{
	parser.ReadVariable("normalColor", normalColor);

	parser.ReadVariable("collidedColor", collidedColor);

	parser.ReadVariable("collidedColorTime", collidedColorTime);
}

void Behaviors::ColorChange::Serialize(Parser & parser) const
{
	parser.WriteVariable("normalColor", normalColor);

	parser.WriteVariable("collidedColor", collidedColor);

	parser.WriteVariable("collidedColorTime", collidedColorTime);
}

void Behaviors::ColorChangeCollisionHandler(GameObject & object, GameObject & other)
{
	UNREFERENCED_PARAMETER(other);
	ColorChange* objectColorChange = object.GetComponent<ColorChange>();

	objectColorChange->collided = true;
	objectColorChange->timer = 0.0f;
}

void Behaviors::ColorChange::Initialize()
{
	sprite = GetOwner()->GetComponent<Sprite>();
	Collider* collider = GetOwner()->GetComponent<Collider>();
	collider->SetCollisionHandler(ColorChangeCollisionHandler);
}

void Behaviors::ColorChange::Update(float dt)
{
	timer -= dt;
	if (collided) {
		timer = collidedColorTime;
		collided = false;
		sprite->SetColor(collidedColor);
	} else if (timer <= 0.0f) {
		sprite->SetColor(normalColor);
	}
}


