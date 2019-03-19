//------------------------------------------------------------------------------
//
// File Name:	Space.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Space.h"
#include "Level.h"
#include "GameObjectManager.h"
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <Graphics.h>
#include <Camera.h>
#include <Engine.h>
#include <SpaceManager.h>

Space::Space(const std::string & name, bool _depth, bool _useFirstSpaceCamera) : BetaObject(name), objectManager(this)
{
	paused = false;
	currentLevel = nullptr;
	nextLevel = nullptr;
	camera = new Camera();
	depth = _depth;
	useFirstSpaceCamera = _useFirstSpaceCamera;
}

Space::~Space()
{
	Shutdown();

	delete camera;
}

void Space::Update(float dt)
{
	//std::cout << "Space::Update" << std::endl;

	if (useFirstSpaceCamera)
	{
		Graphics::GetInstance().SetCurrentCamera(*Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByID(0)->camera);
		Graphics::GetInstance().SetDepthEnabled(Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByID(0)->depth);
	}
	else {
		Graphics::GetInstance().SetCurrentCamera(*camera);
		Graphics::GetInstance().SetDepthEnabled(depth);
	}

	if (nextLevel != nullptr) {
		ChangeLevel();
	}

	if (currentLevel != nullptr && !paused) {
		currentLevel->Update(dt);
		objectManager.Update(dt);
	}
}

void Space::Shutdown()
{
	if (currentLevel != nullptr) {
		currentLevel->Shutdown();
		objectManager.Shutdown();

		currentLevel->Unload();
		objectManager.Unload();

		delete currentLevel;
		currentLevel = nullptr;
	}
}

bool Space::IsPaused() const
{
	return paused;
}

const std::string & Space::GetLevelName() const
{
	return GetName();
}

void Space::SetPaused(bool value)
{
	paused = value;
}

void Space::SetLevel(Level * level)
{
	nextLevel = level;
	nextLevel->SetParent(this);
}

Level * Space::GetLevel()
{
	return currentLevel;
}

void Space::RestartLevel()
{
	SetLevel(currentLevel);
}

GameObjectManager & Space::GetObjectManager()
{
	return objectManager;
}

Camera * Space::GetCamera()
{
	return camera;
}

void Space::ChangeLevel()
{
	//std::cout << "Space::ChangeLevel" << std::endl;

	if (currentLevel == nextLevel) {
		//Restarting Level
		currentLevel->Shutdown();
		objectManager.Shutdown();

		currentLevel->Initialize();
	} else {
		//switching levels
		if (currentLevel != nullptr) {
			currentLevel->Shutdown();
			objectManager.Shutdown();

			currentLevel->Unload();
			objectManager.Unload();
		}
		nextLevel->Load();
		nextLevel->Initialize();

		delete currentLevel;
	}

	currentLevel = nextLevel;
	nextLevel = nullptr;
}
