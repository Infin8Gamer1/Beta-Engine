//------------------------------------------------------------------------------
//
// File Name:	GameObjectPlacer.h
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

#include "Component.h" // base class

#include <Vector2D.h> // Vector2D

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class MenuController;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


class GameObjectPlacer : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	GameObjectPlacer();

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* Clone() const override;

	// Initialize data for this object.
	void Initialize() override;

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Update(float dt) override;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	void PlaceObject(Vector2D mousePosition);

	static void onMouseClickUp(int key);

	MenuController* menuController;

	GameObject* TileMapObject;
	GameObject* menuObject;

	bool enabled;
	static bool clicked;
};


//------------------------------------------------------------------------------
