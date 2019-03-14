#include "stdafx.h"
#include "SaveManager.h"
#include <GameObject.h>
#include <Space.h>
#include <ColliderTilemap.h>
#include <GameObjectManager.h>
#include <Tilemap.h>
#include <System.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw3.h>
#include <glfw3native.h>
#include <Parser.h>
#include <Input.h>
#include <Level.h>

Behaviors::SaveManager::SaveManager() : Component("SaveManager")
{
	level = nullptr;
}

Component * Behaviors::SaveManager::Clone() const
{
	return new SaveManager(*this);
}

void Behaviors::SaveManager::Initialize()
{
}

void Behaviors::SaveManager::Update(float dt)
{
	if (level == nullptr) {
		level = GetOwner()->GetSpace()->GetLevel();
	}

	if (Input::GetInstance().CheckReleased(VK_F1))
	{
		Save();
	}

	if (Input::GetInstance().CheckReleased(VK_F2))
	{
		Load();
	}

	if (Input::GetInstance().CheckReleased(VK_F3))
	{
		Test();
	}

	
}

void Behaviors::SaveManager::Load()
{
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
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		//std::cout << szFile << std::endl;
		std::string fileName = szFile;
		fileName = fileName.substr(fileName.find_last_of("\\") + 1);

		//fileName = fileName.substr(0, fileName.find_first_of("."));

		Parser* parser = new Parser(LevelFilePath + fileName, std::fstream::in);

		level->Deserialize(*parser);

		delete parser;
	}
}

void Behaviors::SaveManager::Save()
{
	if (level == nullptr)
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

		level->Serialize(*parser);

		delete parser;
	}

}

void Behaviors::SaveManager::Test() {

	HWND hwnd = glfwGetWin32Window(System::GetInstance().GetWindowHandle());

	int msgBoxID = MessageBox(
		hwnd,
		"this is a test. I am testing!",
		"Confirm That This is a Test",
		MB_ICONINFORMATION | MB_YESNO | MB_APPLMODAL
	);

	if (msgBoxID == IDYES)
	{
		// TODO: add code
		std::cout << "User Clicked Yes";
	}
	else if (msgBoxID == IDNO) {
		std::cout << "User Clicked NO!";
	}

}