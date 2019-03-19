#pragma once
//------------------------------------------------------------------------------
//
// File Name:	GameObjectButton.h
// Author(s):	Jacob Holyfield
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

class MenuController;
class Sprite;
class SpriteText;
class GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class GameObjectButton : public Button
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	GameObjectButton();

	Component* Clone() const;

	~GameObjectButton();

	// Initialize this component (happens at object creation).
	void Initialize() override;

	void Update(float dt) override;

	//Click function for tab
	void Clicked();

	void SetGameObjectName(std::string Name);

private:

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	MenuController* menuController;
	Sprite* sprite;

	std::string GameObjectName;

	GameObject* TextGO;
	SpriteText* TextSprite;
};