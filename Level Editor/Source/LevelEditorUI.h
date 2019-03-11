//------------------------------------------------------------------------------
//
// File Name:	LevelEditorUI.h
// Author(s):	Jacob Holyfield
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class SpriteSource;
class Texture;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class LevelEditorUI : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Create an instance of the Omega game state.
		LevelEditorUI();

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

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------
	};
}

//----------------------------------------------------------------------------
