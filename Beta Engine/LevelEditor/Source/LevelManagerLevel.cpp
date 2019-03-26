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
#include "LevelManagerLevel.h"
#include "LevelEditorLevel.h"
#include "LevelEditorUI.h"

//Components
#include "CameraMovement.h"
#include "TileMapBrush.h"
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>
#include "MenuController.h"
#include "GameObjectPlacer.h"

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
#include <Parser.h>
#include <FileOpenHelper.h>

#include "../resource.h"

#include <shobjidl.h>
#include <sstream>

WNDPROC Levels::LevelManagerLevel::PreviousWndProc = nullptr;
Levels::LevelManagerLevel* Levels::LevelManagerLevel::Instance = nullptr;

Levels::LevelManagerLevel::LevelManagerLevel() : Level("LevelManager")
{
	uiSpace = nullptr;
	levelSpace = nullptr;
	Instance = this;
}

LRESULT CALLBACK Levels::LevelManagerLevel::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
			std::cout << "Open Menu Clicked" << std::endl;
			Instance->LoadLevel();
			break;
		case ID_FILE_SAVE:
			std::cout << "Save Menu Clicked" << std::endl;
			Instance->SaveLevel();
			break;
		case ID_FILE_OPENGAMEOBJECT:
			std::cout << "Open Game Object Menu Clicked" << std::endl;
			Instance->AddGameObject();
			break;
		}
		break;
	}

	return CallWindowProc(PreviousWndProc, hwnd, uMsg, wParam, lParam);
}

void Levels::LevelManagerLevel::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	Graphics::GetInstance().GetCurrentCamera().Reset();

	GameObjectFactory::GetInstance().RegisterComponent<CameraMovement>();
	GameObjectFactory::GetInstance().RegisterComponent<TileMapBrush>();
	GameObjectFactory::GetInstance().RegisterComponent<GameObjectPlacer>();

	HWND hwnd = glfwGetWin32Window(System::GetInstance().GetWindowHandle());
	PreviousWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&WindowProc);

	//glfwSetCursor(System::GetInstance().GetWindowHandle(), glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));
}

void Levels::LevelManagerLevel::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;

	GameObject* CenterCircle = GameObjectFactory::GetInstance().CreateObject("LevelEditor/UICircle");
	GetSpace()->GetObjectManager().AddObject(*CenterCircle);

	GameObject* Brush = GameObjectFactory::GetInstance().CreateObject("LevelEditor/Brush");
	GetSpace()->GetObjectManager().AddObject(*Brush);

	GameObject* GOPlacer = GameObjectFactory::GetInstance().CreateObject("LevelEditor/GameObjectPlacer");
	GetSpace()->GetObjectManager().AddObject(*GOPlacer);

	GameObject* CameraMovement = GameObjectFactory::GetInstance().CreateObject("LevelEditor/CameraManager");
	GetSpace()->GetObjectManager().AddObject(*CameraMovement);
}

void Levels::LevelManagerLevel::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (uiSpace == nullptr)
	{
		uiSpace = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("UI");
	}

	if (levelSpace == nullptr)
	{
		levelSpace = Engine::GetInstance().GetModule<SpaceManager>()->GetSpaceByName("Level");
	}

	if (Input::GetInstance().CheckReleased(VK_F1))
	{
		SaveLevel();
	}

	if (Input::GetInstance().CheckReleased(VK_F2))
	{
		LoadLevel();
	}

	if (Input::GetInstance().CheckReleased(VK_F3))
	{
		//Test();
		AddGameObject();
	}
}

void Levels::LevelManagerLevel::Shutdown()
{
	std::cout << GetName() << "::Shutdown" << std::endl;
}

void Levels::LevelManagerLevel::Unload()
{
	std::cout << GetName() << "::Unload" << std::endl;
}

void Levels::LevelManagerLevel::LoadLevel()
{
	std::string fileName = FileOpenHelper::BasicFileOpen();

	if (fileName == "")
	{
		return;
	}

	fileName = fileName.substr(fileName.find_last_of("\\") + 1);

	//fileName = fileName.substr(0, fileName.find_first_of("."));

	fileName = LevelFilePath + fileName;

	levelSpace->GetLevel()->SetFileLocation(fileName);

	levelSpace->RestartLevel();

	uiSpace->RestartLevel();

	GetSpace()->RestartLevel();
}

void Levels::LevelManagerLevel::SaveLevel()
{
	if (levelSpace == nullptr)
	{
		return;
	}

	std::string fileName = FileOpenHelper::BasicFileSave();//GetSaveLocation();

	if (fileName == "")
	{
		return;
	}

	fileName = fileName.substr(fileName.find_last_of("\\") + 1);

	//fileName = fileName.substr(0, fileName.find_first_of("."));

	fileName = LevelFilePath + fileName;

	levelSpace->GetLevel()->SetFileLocation(fileName);

	levelSpace->GetLevel()->SaveLevel();
}

void Levels::LevelManagerLevel::AddGameObject()
{
	std::string fileName = FileOpenHelper::BasicFileOpen();

	if (fileName == "")
	{
		return;
	}

	std::string ObjectPath = "Assets\\Objects\\";

	size_t position = fileName.rfind(ObjectPath);

	fileName = fileName.substr(position + ObjectPath.length());

	fileName = fileName.substr(0, fileName.find_first_of("."));

	uiSpace->GetObjectManager().GetObjectByName("MenuController")->GetComponent<MenuController>()->AddGameObjectNames(fileName);
}

void Levels::LevelManagerLevel::Test() {

	HWND hwnd = glfwGetWin32Window(System::GetInstance().GetWindowHandle());

	int msgBoxID = MessageBox(
		hwnd,
		"Yes to load \nNo to save",
		"Do You Want To Save Or Load?",
		MB_ICONINFORMATION | MB_YESNO | MB_APPLMODAL
	);

	if (msgBoxID == IDYES)
	{
		LoadLevel();
	}
	else if (msgBoxID == IDNO) {
		SaveLevel();
	}
}
