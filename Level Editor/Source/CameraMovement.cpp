//------------------------------------------------------------------------------
//
// File Name:	CameraMovement.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "CameraMovement.h"
#include "GameObject.h"
#include <Graphics.h>
#include <Input.h>
#include <Parser.h>

Behaviors::CameraMovement::CameraMovement() : Component("CameraMovement")
{
	up = 'w';
	down = 's';
	left = 'a';
	right = 'd';
	speed = 10.0f;
}

Component * Behaviors::CameraMovement::Clone() const
{
	return new CameraMovement();
}

void Behaviors::CameraMovement::Deserialize(Parser & parser)
{
	parser.ReadVariable("UP", up);
	parser.ReadVariable("DOWN", down);
	parser.ReadVariable("LEFT", left);
	parser.ReadVariable("RIGHT", right);
	parser.ReadVariable("Speed", speed);
}

void Behaviors::CameraMovement::Serialize(Parser & parser) const
{
	parser.WriteVariable("UP", up);
	parser.WriteVariable("DOWN", down);
	parser.WriteVariable("LEFT", left);
	parser.WriteVariable("RIGHT", right);
	parser.WriteVariable("Speed", speed);
}

void Behaviors::CameraMovement::Update(float dt)
{
	
	Vector2D cameraTranslation = Graphics::GetInstance().GetCurrentCamera().GetTranslation();

	if (Input::GetInstance().IsKeyDown(up))
	{
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(cameraTranslation + Vector2D(0, speed));
	}

	if (Input::GetInstance().IsKeyDown(down))
	{
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(cameraTranslation + Vector2D(0, -speed));
	}

	if (Input::GetInstance().IsKeyDown(left))
	{
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(cameraTranslation + Vector2D(-speed, 0));
	}

	if (Input::GetInstance().IsKeyDown(right))
	{
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(cameraTranslation + Vector2D(speed, 0));
	}
}
