//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.h
// Author(s):	 Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include <Serializable.h>

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class Vector2D;
class Sprite;
class SpriteTilemap;
class SpriteText;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class SpriteSource : public Serializable
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new sprite source object.
	// Params:
	//	 numCols = The number of columns in the sprite sheet.
	//	 numRows = The number of rows in the sprite sheet.
	//	 texture = A pointer to a texture that has been loaded by the Alpha Engine.
	SpriteSource(std::string name = "", unsigned numCols = 1, unsigned numRows = 1, unsigned frameCount = 1, unsigned frameStart = 0, Texture* texture = nullptr);

	// Loads object data from a file.
	void Deserialize(Parser& parser) override;

	// Saves object data to a file.
	void Serialize(Parser& parser) const override;

	// Returns a pointer to the texture, for the purposes of rendering a sprite.
	// Returns:
	//	 A pointer to the sprite source texture.
	Texture* GetTexture() const;

	// Sets the texture used by the sprite source.
	void SetTexture(Texture* texture);

	// Returns the maximum number of possible frames in the sprite source's texture (rows * cols).
	unsigned GetFrameCountTexture() const;

	// Returns the number of frames used by this sprite source for animation.
	unsigned GetFrameCount() const;

	// Returns the first frame used when animating.
	unsigned GetFrameStart() const;

	std::string GetName() const;

	void SetName(const std::string &Name);

	// Returns the UV coordinates of the specified frame in a sprite sheet.
	// Params:
	//	 frameIndex = A frame index within a sprite sheet.
	//   textureCoords = Reference to a Vector2D containing the UV/texture coordinates.
	//void GetUV(unsigned int frameIndex, Vector2D& textureCoords) const;

	// Returns the UV coordinates of the specified frame in a sprite sheet.
	// Params:
	//	 frameIndex = A frame index within a sprite sheet.
	// Returns:
	//   A vector containing the UV/texture coordinates.
	const Vector2D GetUV(unsigned int frameIndex) const;

	// Gets the name of the texture (for serialization)
	const std::string& GetTextureName() const;

	// Gets the dimensions of the texture (number of columns, rows)
	const Vector2D GetTextureDimensions() const;

private:
	// The dimensions of the sprite sheet.
	unsigned numRows;
	unsigned numCols;

	// Animation attributes
	unsigned frameCount;
	unsigned frameStart;

	// Pointer to a texture created using the Beta Engine.
	Texture* texture;

	std::string name;
};

//----------------------------------------------------------------------------
