//------------------------------------------------------------------------------
//
// File Name:	Menu.h
// Author(s):	Sage Dupuy
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

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

    // Initialize this component (happens at object creation).
    void Initialize() override;

    // Update function for this component.
    // Params:
    //   dt = The (fixed) change in time since the last step.
    void Update(float dt) override;

    virtual void Clicked() = 0;

private:
    //------------------------------------------------------------------------------
    // Private Variables:
    //------------------------------------------------------------------------------

    // Get Muse Pos
    Vector2D GetMousePosition();

    bool IsClicked();
};