#pragma once

//------------------------------------------------------------------------------
//
// File Name:	SpaceManager.h
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

#include <BetaObject.h>
#include "GameObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Space;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public functions declared in the header.
class SpaceManager : public BetaObject
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor(s)
	SpaceManager(bool CreateTwBar = false);

	// Destructor
	~SpaceManager();

	// Initialize this
	void Initialize() override;

	// Update all objects in the active game objects list.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt) override;

	// Shutdown the game object manager, destroying all active objects.
	void Shutdown(void) override;

	// Add a game object to the active game object list.
	// Params:
	//	 gameObject = Reference to the game object to be added to the list.
	void AddSpace(Space& space);

	// Returns a pointer to the first active game object matching the specified name.
	// Params:
	//	 objectName = The name of the object to be returned, if found.
	// Returns:
	//   If the named object is found,
	//	   then return the pointer to the named game object,
	//	   else return nullptr.
	Space* GetSpaceByName(const std::string& spaceName) const;

	// Returns a pointer to the first active game object matching the specified name.
	// Params:
	//	 objectName = The name of the object to be returned, if found.
	// Returns:
	//   If the named object is found,
	//	   then return the pointer to the named game object,
	//	   else return nullptr.
	Space* GetSpaceByID(size_t spaceID) const;

	// Returns the number of active objects with the given name.
	// Params:
	//   objectName = The name of the objects that should be counted.
	unsigned GetSpaceCount() const;

	CTwBar* GetTwBar();

	TwType GetVector2DTwType();

	TwType GetColorTwType();

private:
	// Disable copy constructor and assignment operator
	SpaceManager(const SpaceManager&) = delete;
	SpaceManager& operator=(const SpaceManager&) = delete;

	// Objects
	std::vector<Space*> spaceList;

	bool CreateTwBar;
	CTwBar* bar;
	TwType TW_TYPE_VECTOR2D;
	TwType TW_TYPE_BETA_COLOR;
};

//------------------------------------------------------------------------------
