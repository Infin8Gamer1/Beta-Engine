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

enum ToolType
{
	TMBrush,
	GOPlacer,
	GOSelecter
};

class MenuController : public Component
{
public:
    //------------------------------------------------------------------------------
    // Public Functions:
    //------------------------------------------------------------------------------

    MenuController();

	~MenuController();

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
    //void Update(float dt) override;

	bool IsMouseOnUI();

    void ShowMenu(GameObject* menu);

    void ShiftTabsPos();

    void RestoreTabsPos();

	int GetSelectedTile();

	void SetSelectedTile(int ID);

	std::string GetSelectedGameObjectName();

	void SetSelectedGameObjectName(std::string name);

	void AddGameObjectNames(std::string name);

	std::vector<std::string> GetGameObjectNames();

	ToolType GetEnabledTool();

	void SetEnabledTool(ToolType tool);

private:

    //------------------------------------------------------------------------------
    // Private Variables:
    //------------------------------------------------------------------------------

    int menuCount;
    std::vector<GameObject*> menus;
    std::vector<Button*> buttons;

    int tabBuffer;

	int SelectedTileID;
	std::string SelectedGameObjectTemplateName;

	std::vector<std::string> GameObjectNames;

	ToolType EnabledTool;
	TwType TW_TYPE_TOOL_TYPE;
};