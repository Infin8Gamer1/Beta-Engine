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


Behaviors::SaveManager::SaveManager() : Component("SaveManager")
{
}

Component * Behaviors::SaveManager::Clone() const
{
	return new SaveManager(*this);
}

void Behaviors::SaveManager::Initialize()
{
	map = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("TileMap")->GetComponent<ColliderTilemap>()->GetTilemap();
}

void Behaviors::SaveManager::Update(float dt)
{
	if (Input::GetInstance().CheckReleased(VK_F1))
	{
		Save();
	}

	if (Input::GetInstance().CheckReleased(VK_F2))
	{
		Load();
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
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		//std::cout << szFile << std::endl;

		Parser* parser = new Parser(szFile, std::fstream::in);

		map->Deserialize(*parser);
	}
}

void Behaviors::SaveManager::Save()
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
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	// Display the Save dialog box. 

	if (GetSaveFileName(&ofn) == TRUE)
	{
		//std::cout << szFile << std::endl;

		Parser* parser = new Parser(szFile, std::fstream::out);

		map->Serialize(*parser);
	}

}