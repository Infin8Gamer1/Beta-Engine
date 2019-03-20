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
#include <Graphics.h>
#include <Input.h>
#include <Parser.h>
#include <Camera.h>
#include <System.h>
#include <Space.h>
#include <Camera.h>
#include <Engine.h>
#include <SpaceManager.h>
#include <GameObject.h>
#include <CallbackInputManager.h>

CameraMovement::CameraMovement() : Component("CameraMovement")
{
	up = 'W';
	down = 'S';
	left = 'A';
	right = 'D';
	in = 'E';
	out = 'Q';
	zoomSpeed = 1.5f;
	speed = 10.0f;
}

Component * CameraMovement::Clone() const
{
	return new CameraMovement(*this);
}

void CameraMovement::Deserialize(Parser & parser)
{
	parser.ReadVariable("UP", up);
	parser.ReadVariable("DOWN", down);
	parser.ReadVariable("LEFT", left);
	parser.ReadVariable("RIGHT", right);
	parser.ReadVariable("Speed", speed);
}

void CameraMovement::Serialize(Parser & parser) const
{
	parser.WriteVariable("UP", up);
	parser.WriteVariable("DOWN", down);
	parser.WriteVariable("LEFT", left);
	parser.WriteVariable("RIGHT", right);
	parser.WriteVariable("Speed", speed);
}

void CameraMovement::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	Camera* camera = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level")->GetCamera();

	int MouseWheelY = CallbackInputManager::GetInstance().GetCursorScroll();

	Vector2D cameraTranslation = camera->GetTranslation();
	float cameraFOV = camera->GetFOV();

	if (Input::GetInstance().IsKeyDown(up))
	{
		cameraTranslation = cameraTranslation + Vector2D(0, speed);
	}

	if (Input::GetInstance().IsKeyDown(down))
	{
		cameraTranslation = cameraTranslation + Vector2D(0, -speed);
	}

	if (Input::GetInstance().IsKeyDown(left))
	{
		cameraTranslation = cameraTranslation + Vector2D(-speed, 0);
	}

	if (Input::GetInstance().IsKeyDown(right))
	{
		cameraTranslation =  cameraTranslation + Vector2D(speed, 0);
	}

	if (Input::GetInstance().IsKeyDown(in))
	{
		cameraFOV = cameraFOV - zoomSpeed;
	}

	if (Input::GetInstance().IsKeyDown(out))
	{
		cameraFOV = cameraFOV + zoomSpeed;
	}

	if (previousMouseWheelY == MouseWheelY)
	{
		MouseWheelY = 0;
		previousMouseWheelY = MouseWheelY;
	}
	else {
		cameraFOV = cameraFOV + MouseWheelY * -5;
		previousMouseWheelY = MouseWheelY;
	}

	camera->SetTranslation(cameraTranslation);
	camera->SetFOV(cameraFOV);
}
