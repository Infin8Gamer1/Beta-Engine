//------------------------------------------------------------------------------
//
// File Name:	LevelEditorLevel.cpp
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
#include "LevelEditorUI.h"

//Components
#include "ScreenWrap.h"
#include "TimedDeath.h"
#include "CameraMovement.h"
#include "TileMapBrush.h"
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>

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
#include <SpaceManager.h>
#include <Graphics.h>

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
	Graphics::GetInstance().GetCurrentCamera().Reset();

	////Register Custom Components
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::CameraMovement>();
	GameObjectFactory::GetInstance().RegisterComponent<TileMapBrush>();

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

	GameObject* Brush = GameObjectFactory::GetInstance().CreateObject("Brush");
	GetSpace()->GetObjectManager().AddObject(*Brush);

	GameObject* CameraMovement = GameObjectFactory::GetInstance().CreateObject("CameraManager");
	GetSpace()->GetObjectManager().AddObject(*CameraMovement);
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

	soundManager->Shutdown();
}