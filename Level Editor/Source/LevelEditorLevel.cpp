//------------------------------------------------------------------------------
//
// File Name:	Omega.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "LevelEditorLevel.h"

//Components
#include "ScreenWrap.h"
#include "TimedDeath.h"
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>

//Resources
#include <Mesh.h>
#include <Color.h>

//Systems
#include <GameObjectFactory.h>
#include <ResourceManager.h>
#include <System.h>
#include <Input.h>
#include <MeshHelper.h>
#include <Engine.h>
#include <SoundManager.h>
#include <Random.h>

Levels::LevelEditorLevel::LevelEditorLevel() : Level("LevelEditor")
{
	// Sound manager
	soundManager = nullptr;
	musicChannel = nullptr;
}

void Levels::LevelEditorLevel::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	System::GetInstance().SetWindowTitle(WindowTitle);

	////Register Custom Components
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::TimedDeath>();
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::Bullet>();
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::PlayerShip>();
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::Asteroid>();

	//GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Bullet"));

	//GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Asteroid"));

	//Setup Sounds
	soundManager = Engine::GetInstance().GetModule<SoundManager>();
	soundManager->AddMusic("Asteroid_Field.mp3");
	soundManager->AddEffect("teleport.wav");

	soundManager->AddBank("Master Bank.strings.bank");
	soundManager->AddBank("Master Bank.bank");
}

void Levels::LevelEditorLevel::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;

	////arena setup
	//GameObject* walls = GameObjectFactory::GetInstance().CreateObject("Walls");
	//GetSpace()->GetObjectManager().AddObject(*walls);

	//GameObject* Line = GameObjectFactory::GetInstance().CreateObject("Line");
	//GetSpace()->GetObjectManager().AddObject(*Line);

	//GameObject* text = GameObjectFactory::GetInstance().CreateObject("ScoreText");
	//scoreText = text->GetComponent<SpriteText>();
	//GetSpace()->GetObjectManager().AddObject(*text);

	//GameObject* instructionText = GameObjectFactory::GetInstance().CreateObject("Instructions");
	//GetSpace()->GetObjectManager().AddObject(*instructionText);

	//GameObject* Ship = GameObjectFactory::GetInstance().CreateObject("Ship");
	//playerShip = Ship->GetComponent<Behaviors::PlayerShip>();
	//GetSpace()->GetObjectManager().AddObject(*Ship);

	//play background music
	musicChannel = soundManager->PlaySound("Asteroid Field");
}

void Levels::LevelEditorLevel::Update(float dt)
{
	
}

void Levels::LevelEditorLevel::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;

	musicChannel->stop();
	musicChannel = nullptr;
}

void Levels::LevelEditorLevel::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;

	ResourceManager::GetInstance().Shutdown();

	soundManager->Shutdown();
}