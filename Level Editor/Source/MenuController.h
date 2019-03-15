#pragma once
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
#include <Vector>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class Vector2D;
class TileMapBrush;
class Button;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class MenuController : public Component
{
public:
    //------------------------------------------------------------------------------
    // Public Functions:
    //------------------------------------------------------------------------------

    MenuController();

    // Clone a component and return a pointer to the cloned component.
    // Returns:
    //   A pointer to a dynamically allocated clone of the component.
    Component* Clone() const override;

    // Write object data to file
    // Params:
    //   parser = The parser that is writing this object to a file.
    void Serialize(Parser& parser) const override;

    // Read object data from a file
    // Params:
    //   parser = The parser that is reading this object's data from a file.
    void Deserialize(Parser& parser) override;

    // Initialize this component (happens at object creation).
    void Initialize() override;

    // Update function for this component.
    // Params:
    //   dt = The (fixed) change in time since the last step.
    void Update(float dt) override;

    void ShowMenu(GameObject* menu);

    void ShiftTabsPos();

    void RestoreTabsPos();

	void Refresh();

private:

    //------------------------------------------------------------------------------
    // Private Variables:
    //------------------------------------------------------------------------------

    int menuCount;
    std::vector<GameObject*> menus;
    std::vector<Button*> buttons;

    int tabBuffer;

    TileMapBrush* brush;
};