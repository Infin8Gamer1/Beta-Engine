#pragma once
//------------------------------------------------------------------------------
//
// File Name:	TileButton.h
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

class TileMapBrush;
class Sprite;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class TileButton : public Button
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	TileButton();

	Component* Clone() const;

	// Initialize this component (happens at object creation).
	void Initialize() override;

	//Click function for tab
	void Clicked();

	void SetBrush(TileMapBrush* brush);

	void SetTileID(int ID);

private:

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	TileMapBrush* brush;
	Sprite* sprite;

	int TileID;
};