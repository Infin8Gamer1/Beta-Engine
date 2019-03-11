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

Space::Space(const std::string & name) : BetaObject(name), objectManager(this)
{
	paused = false;
	currentLevel = nullptr;
	nextLevel = nullptr;
}

Space::~Space()
{
	Shutdown();
}

void Space::Update(float dt)
{
	//std::cout << "Space::Update" << std::endl;

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

void Space::RestartLevel()
{
	SetLevel(currentLevel);
}

GameObjectManager & Space::GetObjectManager()
{
	return objectManager;
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
