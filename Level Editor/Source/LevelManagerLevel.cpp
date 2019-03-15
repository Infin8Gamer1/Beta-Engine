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
#include "ScreenWrap.h"
#include "TimedDeath.h"
#include "CameraMovement.h"
#include "TileMapBrush.h"
#include <SpriteText.h>
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>
#include "MenuController.h"

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

//GLFW STUFF
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw3.h>
#include <glfw3native.h>

Levels::LevelManagerLevel::LevelManagerLevel() : Level("LevelManager")
{
	uiSpace = nullptr;
	levelSpace = nullptr;
}

void Levels::LevelManagerLevel::Load()
{
	std::cout << GetName() << "::Load" << std::endl;

	Graphics::GetInstance().GetCurrentCamera().Reset();
}

void Levels::LevelManagerLevel::Initialize()
{
	std::cout << GetName() << "::Initialize" << std::endl;
}

void Levels::LevelManagerLevel::Update(float dt)
{
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
		Test();
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
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	HWND hwnd = glfwGetWin32Window(System::GetInstance().GetWindowHandle());              // owner window
	std::string fileName = "";

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Level\0*.lvl\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	levelSpace->GetObjectManager().DestroyAllObjects();

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		//std::cout << szFile << std::endl;
		fileName = szFile;
		fileName = fileName.substr(fileName.find_last_of("\\") + 1);

		fileName = LevelFilePath + fileName;

		Parser* parser = new Parser(fileName, std::fstream::in);

		levelSpace->GetLevel()->Deserialize(*parser);

		delete parser;

		levelSpace->GetLevel()->Initialize();

		levelSpace->GetObjectManager().GetObjectByName("Brush")->GetComponent<TileMapBrush>()->Initialize();

		uiSpace->RestartLevel();

		//fileName = fileName.substr(0, fileName.find_first_of("."));
	}
}

void Levels::LevelManagerLevel::SaveLevel()
{
	if (levelSpace == nullptr)
	{
		return;
	}

	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	HWND hwnd = glfwGetWin32Window(System::GetInstance().GetWindowHandle());              // owner window

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Level\0*.lvl\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	// Display the Save dialog box. 

	if (GetSaveFileName(&ofn) == TRUE)
	{
		//std::cout << szFile << std::endl;
		std::string fileName = szFile;
		fileName = fileName.substr(fileName.find_last_of("\\") + 1);

		//fileName = fileName.substr(0, fileName.find_first_of("."));

		Parser* parser = new Parser(LevelFilePath + fileName, std::fstream::out);

		levelSpace->GetLevel()->Serialize(*parser);

		delete parser;
	}

}

void Levels::LevelManagerLevel::Test() {

	HWND hwnd = glfwGetWin32Window(System::GetInstance().GetWindowHandle());

	int msgBoxID = MessageBox(
		hwnd,
		"Yes to load \nNo to save",
		"Confirm That This is a Test",
		MB_ICONINFORMATION | MB_YESNO | MB_APPLMODAL
	);

	if (msgBoxID == IDYES)
	{
		levelSpace->GetObjectManager().DestroyAllObjects();
		uiSpace->GetObjectManager().DestroyAllObjects();

		Parser* parser = new Parser("Assets/Level.lvl", std::fstream::in);

		levelSpace->GetLevel()->Deserialize(*parser);

		delete parser;

		levelSpace->GetLevel()->Initialize();

		uiSpace->RestartLevel();

		std::cout << "Loaded";
	}
	else if (msgBoxID == IDNO) {
		Parser* parser = new Parser("Assets/Level1.lvl", std::fstream::out);

		levelSpace->GetLevel()->Serialize(*parser);

		std::cout << "Saved";
	}

	

}