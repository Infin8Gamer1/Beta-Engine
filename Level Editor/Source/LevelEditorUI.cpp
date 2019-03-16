//------------------------------------------------------------------------------
//
// File Name:	LevelEditorUI.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <Space.h>
#include "LevelEditorUI.h"

//Components
#include <SpriteText.h>
#include <Transform.h>
#include "Menu.h"
#include "MenuController.h"
#include "Tab.h"
#include "TileButton.h"
#include "TileMapBrush.h"

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
#include <Graphics.h>
#include "CameraMovement.h"

Levels::LevelEditorUI::LevelEditorUI() : Level("UI")
{
	
}

void Levels::LevelEditorUI::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	Graphics::GetInstance().GetCurrentCamera().Reset();

	////Register Custom Components
	GameObjectFactory::GetInstance().RegisterComponent<Menu>();
    GameObjectFactory::GetInstance().RegisterComponent<MenuController>();
    GameObjectFactory::GetInstance().RegisterComponent<Tab>();
	GameObjectFactory::GetInstance().RegisterComponent<TileButton>();
}

void Levels::LevelEditorUI::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;

	GameObject* MenuController = GameObjectFactory::GetInstance().CreateObject("MenuController");
	GetSpace()->GetObjectManager().AddObject(*MenuController);
}

void Levels::LevelEditorUI::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void Levels::LevelEditorUI::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;
}

void Levels::LevelEditorUI::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;
}