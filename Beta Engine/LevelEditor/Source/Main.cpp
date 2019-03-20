//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

// BetaFramework Engine
#include <Engine.h>

// Engine modules
#include "Space.h"
#include <SoundManager.h>
#include <SpaceManager.h>

// Initial game state
#include "LevelEditorLevel.h"
#include "LevelManagerLevel.h"
#include "LevelEditorUI.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR command_line, _In_ int show)
{
	// Enable memory leak checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// Unused parameters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(show);
	UNREFERENCED_PARAMETER(instance);

	// Create a new space called "Level"
	Space* space = new Space("Level", true);
	space->SetLevel(new Levels::LevelEditorLevel());

	//setup the UI Space
	Space* uiSpace = new Space("UI");
	uiSpace->SetLevel(new Levels::LevelEditorUI());

	// Create a new space called "Management"
	Space* managmentSpace = new Space("Management", true, true);
	managmentSpace->SetLevel(new Levels::LevelManagerLevel());

	SpaceManager* spaceManager = new SpaceManager(true);
	spaceManager->AddSpace(*space);
	spaceManager->AddSpace(*uiSpace);
	spaceManager->AddSpace(*managmentSpace);

	Engine::GetInstance().AddModule(spaceManager);

	// Add Sound Manager
	Engine::GetInstance().AddModule(new SoundManager());

	Engine::GetInstance().SetCloseOnEscape(false);

	// Game engine go!
	int Height = 720;
	int Width = 1280;
	Engine::GetInstance().Start(Width, Height, 200);

	return 0;
}
