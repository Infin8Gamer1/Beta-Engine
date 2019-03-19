//------------------------------------------------------------------------------
//
// File Name:	Button.h
// Author(s):	Sage Dupuy
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Component.h> // base class

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Button : public Component
{
public:
    //------------------------------------------------------------------------------
    // Public Functions:
    //------------------------------------------------------------------------------

    Button(std::string Name);

    // Update function for this component.
    // Params:
    //   dt = The (fixed) change in time since the last step.
    void Update(float dt) override;

	bool getIsHovered();

    virtual void Clicked() = 0;

    void setEnabled(bool value);

    bool getEnabled();

private:
    //------------------------------------------------------------------------------
    // Private Variables:
    //------------------------------------------------------------------------------

    // Get Muse Pos
    Vector2D GetMousePosition();

    bool IsMouseHovering();

    bool enabled;
	
	bool isHovered;
};