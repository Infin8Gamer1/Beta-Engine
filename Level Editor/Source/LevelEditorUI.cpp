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

Levels::LevelEditorUI::LevelEditorUI() : Level("UI")
{
	
}

void Levels::LevelEditorUI::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	////Register Custom Components
	GameObjectFactory::GetInstance().RegisterComponent<Menu>();
}

void Levels::LevelEditorUI::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;

	GameObject* Menu = GameObjectFactory::GetInstance().CreateObject("Menu");
    
	GetSpace()->GetObjectManager().AddObject(*Menu);
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