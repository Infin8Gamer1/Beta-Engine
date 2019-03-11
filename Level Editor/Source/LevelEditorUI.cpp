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
#include "LevelEditorUI.h"

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

Levels::LevelEditorUI::LevelEditorUI() : Level("UI")
{
	
}

void Levels::LevelEditorUI::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	////Register Custom Components
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::TimedDeath>();
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::Bullet>();
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::PlayerShip>();
	//GameObjectFactory::GetInstance().RegisterComponent<Behaviors::Asteroid>();

	//GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Bullet"));

	//GetSpace()->GetObjectManager().AddArchetype(*GameObjectFactory::GetInstance().CreateObject("Asteroid"));
}

void Levels::LevelEditorUI::Initialize()
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
}

void Levels::LevelEditorUI::Update(float dt)
{

}

void Levels::LevelEditorUI::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;
}

void Levels::LevelEditorUI::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;
}