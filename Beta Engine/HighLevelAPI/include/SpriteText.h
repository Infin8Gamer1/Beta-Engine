//------------------------------------------------------------------------------
//
// File Name:	SpriteText.h
// Author(s):	David Cohen (david.cohen), Jacob Holyfield (jacob.holyfield)
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Component.h>
#include <Sprite.h>
#include "Color.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Transform;
class Mesh;
class SpriteSource;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class SpriteText : public Sprite
{
public:
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	enum class Alignment
	{
		CENTER = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3,
		LEFT = 4
	};

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a new sprite object.
	SpriteText(std::string text = "");

	Component* Clone() const override;

	// Loads object data from a file.
	void Deserialize(Parser& parser) override;

	// Saves object data to a file.
	void Serialize(Parser& parser) const override;

	// Draw a sprite (Sprite can be textured or untextured).
	void Draw() override;

	// Sets the string to display.
	// Params:
	//   text = New string to display.
	void SetText(const std::string& text);

	// Get the current string being desplayed.
	std::string GetText() const;

	// Set a sprite's transparency (between 0.0f and 1.0f).
	// Params:
	//   alpha = New value for the sprite's 'alpha' value.
	void SetAlpha(float alpha);

	// Get the current value for a sprite's transparency.
	float GetAlpha() const;

	// Gets the current horizontal alignment.
	Alignment GetHorizontalAlignment() const;

	// Sets the current horizontal alignment.
	void SetHorizontalAlignment(Alignment alignment);

	// Gets the current vertical alignment.
	Alignment GetVerticalAlignment() const;

	// Sets the current vertical alignment.
	void SetVerticalAlignment(Alignment alignment);

	// Calculates the width of the current text when displayed.
	// Returns:
	//   The width of the current text.
	float GetWidth() const;

	// Calculates the height of the current text when displayed.
	// Returns:
	//   The height of the current text.
	float GetHeight() const;

private:
	// Properties (save to/load from file)
	std::string text;

	// The number of columns & rows in the font atlas.
	unsigned columns;
	unsigned rows;

	// The alignment of the text (where the origin of the transform is).
	Alignment horizontalAlignment;
	Alignment verticalAlignment;
};

//------------------------------------------------------------------------------
