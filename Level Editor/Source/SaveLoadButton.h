#pragma once
//------------------------------------------------------------------------------
//
// File Name:	SaveLoadButton.h
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

#include "Button.h" // base class

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

enum state { Save, Load };

class SaveLoadButton : public Button
{
public:
    //------------------------------------------------------------------------------
    // Public Functions:
    //------------------------------------------------------------------------------

    SaveLoadButton(state state = Save);

    // Initialize this component (happens at object creation).
    void Initialize() override;

    Component* Clone() const override;

    // Write object data to file
    // Params:
    //   parser = The parser that is writing this object to a file.
    void Serialize(Parser& parser) const override;

    // Read object data from a file
    // Params:
    //   parser = The parser that is reading this object's data from a file.
    void Deserialize(Parser& parser) override;

    //Click function for Saving
    void Clicked();

private:

    //------------------------------------------------------------------------------
    // Private Variables:
    //------------------------------------------------------------------------------

    state myState;

    GameObject* saveManager;
};