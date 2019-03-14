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
#include <Camera.h>
#include <glfw3.h>
#include <System.h>
#include <Space.h>
#include <Camera.h>

int Behaviors::CameraMovement::MouseWheelY = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Behaviors::CameraMovement::MouseWheelY = yoffset;
}

Behaviors::CameraMovement::CameraMovement() : Component("CameraMovement")
{
	up = 'W';
	down = 'S';
	left = 'A';
	right = 'D';
	in = 'E';
	out = 'Q';
	zoomSpeed = 1.5f;
	speed = 10.0f;

	MouseWheelY = 0;
	previousMouseWheelY = 0;
}

Component * Behaviors::CameraMovement::Clone() const
{
	return new CameraMovement(*this);
}

void Behaviors::CameraMovement::Initialize()
{
	glfwSetScrollCallback(System::GetInstance().GetWindowHandle(), scroll_callback);
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
	float cameraFOV = Graphics::GetInstance().GetCurrentCamera().GetFOV();

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
		cameraFOV = cameraFOV + MouseWheelY * 5;
		previousMouseWheelY = MouseWheelY;
	}

	Graphics::GetInstance().GetCurrentCamera().SetTranslation(cameraTranslation);
	Graphics::GetInstance().GetCurrentCamera().SetFOV(cameraFOV);
}
