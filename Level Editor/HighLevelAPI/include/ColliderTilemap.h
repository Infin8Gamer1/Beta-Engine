//------------------------------------------------------------------------------
//
// File Name:	ColliderTilemap.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Collider.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Tilemap;
struct BoundingRectangle;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Enum for sides of bounding rectangle
enum RectangleSide
{
	SideBottom,
	SideTop,
	SideLeft,
	SideRight,

	// Number of sides
	SideCount,
};

// Tilemap collider class - goes on Tilemap object
class ColliderTilemap : public Collider
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for Tilemap collider.
	ColliderTilemap();

	// Clone an collider and return a pointer to the cloned object.
	// Returns:
	//   A pointer to a collider.
	Component* Clone() const override;

	// Loads object data from a file.
	void Deserialize(Parser& parser) override;

	// Saves object data to a file.
	void Serialize(Parser& parser) const override;

	// Debug drawing for colliders.
	void Draw() override;

	// Check for collision between a tilemap and another arbitrary collider.
	// Params:
	//	 other = Reference to the second collider component.
	// Returns:
	//	 Return the results of the collision check.
	bool IsCollidingWith(const Collider& other) const override;

	// Sets the tilemap to use for this collider.
	// Params:
	//   map = A pointer to the tilemap resource.
	void SetTilemap(Tilemap* map);

	Tilemap* GetTilemap();

	Vector2D ConvertTileMapCordsToWorldCords(Vector2D inputCords);

	Vector2D ConvertWorldCordsToTileMapCords(Vector2D inputCords);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Checks whether the specified side of a given rectangle is colliding with the tilemap.
	// Params:
	//   rectangle = The bounding rectangle for an object.
	//   side = The side that needs to be tested for collision.
	// Returns:
	//   True if there is a collision, false otherwise.
	bool IsSideColliding(const BoundingRectangle& rectangle, RectangleSide side) const;

	// Determines whether a point is within a collidable cell in the tilemap.
	// Params:
	//   x = The x component of the point, in world coordinates.
	//   y = The y component of the point, in world coordinates.
	// Returns:
	//   False if the point is outside the map or the map is empty at that position, 
	//   or true if there is a tile at that position.
	bool IsCollidingAtPosition(float x, float y) const;

	// Moves an object and sets its velocity based on where it collided with the tilemap.
	// Params:
	//   objectRectangle = A bounding rectangle that encompasses the object.
	//   objectTransform = Pointer to the object's transform component.
	//   objectPhysics = Pointer to the object's physics component.
	//   collisions = True/false values from map collision checks.
	// Returns:
	//   False if the point is outside the map or the map is empty at that position, 
	//   or true if there is a tile at that position.
	void ResolveCollisions(const BoundingRectangle& objectRectangle, Transform* objectTransform, 
		Physics* objectPhysics, const MapCollision& collisions) const;

	// Find the center of the closest tile on the x-axis or y-axis.
	// Used for assisting in collision resolution on a particular side.
	// Params:
	//   side = Which side the collision is occurring on.
	//   sidePosition = The x or y value of that side.
	// Returns:
	//   The center of the closest tile to the given position on the given side.
	float GetNextTileCenter(RectangleSide side, float sidePosition) const;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The tilemap
	Tilemap* map;
};

//------------------------------------------------------------------------------
