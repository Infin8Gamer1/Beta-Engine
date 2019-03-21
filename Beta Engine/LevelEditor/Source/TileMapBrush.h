//------------------------------------------------------------------------------
//
// File Name:	Brush.h
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

class Tilemap;
class MenuController;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------


class TileMapBrush : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	TileMapBrush();

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

	void SetTilemap(Tilemap * _map);

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	static void onMouseClick(int key);

	static void onMouseClickUp(int key);

	void PlaceTile(Vector2D mousePosition);

	Tilemap* map;
	GameObject* TileMapObject;

	GameObject* menuObject;
	MenuController* menuController;

	bool enabled;
	static bool clicking;
};


//------------------------------------------------------------------------------
