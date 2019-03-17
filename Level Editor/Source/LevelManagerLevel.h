//------------------------------------------------------------------------------
//
// File Name:	LevelEditorLevel.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Level.h>
#include <fmod.hpp>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class LevelManagerLevel : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create an instance of the Omega game state.
		LevelManagerLevel();

		// Load archetypes, meshes
		void Load();

		// Initialize the memory associated with the Omega game state.
		void Initialize();

		// Update the Omega game state.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt);

		// Set high score
		void Shutdown();

		// Unload textures, sprite sources, meshes
		void Unload();

		void LoadLevel();

		void SaveLevel();

		void AddGameObject();

		void Test();

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		std::string BasicFileOpen();

		std::string BasicFileSave();

		static LRESULT CALLBACK WindowProc(
			_In_ HWND   hwnd,
			_In_ UINT   uMsg,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------
		Space* uiSpace;
		Space* levelSpace;

		std::string LevelFilePath = "Assets/";

		static WNDPROC PreviousWndProc;
		static Levels::LevelManagerLevel* Instance;
	};
}

//----------------------------------------------------------------------------
