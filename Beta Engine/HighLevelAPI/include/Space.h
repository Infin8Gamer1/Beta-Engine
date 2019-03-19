//------------------------------------------------------------------------------
//
// File Name:	Space.h
// Author(s):	Jeremy Kings
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

#include "GameObjectManager.h"
#include <BetaObject.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Level;
class Camera;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Space : public BetaObject
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor(s)
	Space(const std::string& name, bool depth = false, bool useFirstSpaceCamera = false);

	// Destructor
	~Space();

	// Updates the state manager and object manager.
	// Params:
	//   dt = The change in time since the last call to this function.
	void Update(float dt);

	// Shuts down the object manager
	void Shutdown();

	// Returns a boolean indicating whether objects in this space are paused.
	// Returns:
	//   True if the space is paused, false otherwise.
	bool IsPaused() const;

	// Returns the name of the level currently running in this space.
	const std::string& GetLevelName() const;

	// Pauses the space, preventing objects from being updated, but objects are still drawn.
	// Params:
	//   value = A true/false value that indicates whether the space should be paused.
	void SetPaused(bool value);

	// Sets the level that the space is using after unloading the current level.
	// Params:
	//   level = The next level that the space will be using.
	void SetLevel(Level* level);

	// Sets the level that the space is using after unloading the current level.
	// Params:
	//   level = The next level that the space will be using.
	template<class T>
	void SetLevel() {
		SetLevel(new T());
	}

	Level* GetLevel();

	// Restarts the current level (next level = current)
	void RestartLevel();
	
	// Returns the object manager, which you can use to retrieve and add objects.
	GameObjectManager& GetObjectManager();

	// Returns the camera for the space
	Camera* GetCamera();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Updates current/next level pointers and calls shutdown/unload/load/init
	void ChangeLevel();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	bool paused;
	Level* currentLevel;
	Level* nextLevel;
	GameObjectManager objectManager;
	Camera* camera;
	bool depth;
	bool useFirstSpaceCamera;
};

//------------------------------------------------------------------------------
