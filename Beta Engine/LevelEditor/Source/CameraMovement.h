//------------------------------------------------------------------------------
//
// File Name:	CameraMovement.h
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class CameraMovement : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
		
	// Constructor
	// Params:
	CameraMovement();

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* Clone() const override;

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Update(float dt) override;

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Serialize(Parser& parser) const override;

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Deserialize(Parser& parser) override;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	char up;
	char down;
	char left;
	char right;
	char in;
	char out;
	float zoomSpeed;
	float speed;

	int previousMouseWheelY;
};


//------------------------------------------------------------------------------
