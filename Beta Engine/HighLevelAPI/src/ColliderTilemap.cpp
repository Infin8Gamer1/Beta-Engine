//------------------------------------------------------------------------------
//
// File Name:	ColliderTilemap.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderTilemap.h"
#include "Transform.h"
#include "Physics.h"
#include "GameObject.h"
#include "ColliderRectangle.h"
#include <Shapes2D.h>
#include "Matrix2DStudent.h"
#include "Tilemap.h"
#include <ResourceManager.h>
#include <Parser.h>

ColliderTilemap::ColliderTilemap() : Collider(ColliderType::ColliderTypeTilemap)
{
	map = nullptr;
}

Component * ColliderTilemap::Clone() const
{
	return new ColliderTilemap(*this);
}

void ColliderTilemap::Deserialize(Parser & parser)
{
	BaseDeserialize(parser);

	std::string tilemapLoc;
	parser.ReadVariable("tilemapLoc", tilemapLoc);

	SetTilemap(ResourceManager::GetInstance().GetTilemap(tilemapLoc, true));
}

void ColliderTilemap::Serialize(Parser & parser) const
{
	BaseSerialize(parser);

	parser.WriteVariable("tilemapLoc", map->GetName());

	ResourceManager::GetInstance().SaveTilemapToFile(map);
}

void ColliderTilemap::Draw()
{
	//TODO::DebugDraw
}

bool ColliderTilemap::IsCollidingWith(const Collider & other) const
{
	//get components
	Transform* otherTransform = other.GetOwner()->GetComponent<Transform>();;
	Physics* otherPhysics = other.GetOwner()->GetComponent<Physics>();;

	//check if it is a rectangle if it isn't then just return false and don't bother doing anything
	if (other.GetType() != ColliderType::ColliderTypeRectangle || otherPhysics == nullptr) {
		return false;
	}
	
	//create a bounding box for the other object
	BoundingRectangle otherRect = BoundingRectangle(otherTransform->GetTranslation(), static_cast<const ColliderRectangle&>(other).GetExtents());

	//check collision sides and store it in a MapCollision var
	bool bottom = IsSideColliding(otherRect, RectangleSide::SideBottom);
	bool left = IsSideColliding(otherRect, RectangleSide::SideLeft);
	bool right = IsSideColliding(otherRect, RectangleSide::SideRight);
	bool top = IsSideColliding(otherRect, RectangleSide::SideTop);
	MapCollision mapCollision = MapCollision(bottom, top, left, right);

	//Resolve Collisions
	ResolveCollisions(otherRect, otherTransform, otherPhysics, mapCollision);

	if (mapCollision.bottom || mapCollision.left || mapCollision.right || mapCollision.top) {
		if (other.GetMapCollisionHandler() != nullptr) {
			//call collision handler for the other object
			other.GetMapCollisionHandler()(*other.GetOwner(), mapCollision);
		}
		//we collided so return true
		return true;
	}

	//we didn't collide so return false
	return false;
}

void ColliderTilemap::SetTilemap(Tilemap * _map)
{
	map = _map;
}

Tilemap * ColliderTilemap::GetTilemap()
{
	return map;
}

bool ColliderTilemap::IsSideColliding(const BoundingRectangle & rectangle, RectangleSide side) const
{
	bool isCollidingOnSide = false;
	Vector2D hotspots[3];

	float offset = 1.25f;

	if (side == RectangleSide::SideBottom) {
		//create 3 hotspots Left, Middle, Right
		hotspots[0] = Vector2D((rectangle.center.x + rectangle.extents.x / offset), rectangle.bottom);
		hotspots[1] = Vector2D(rectangle.center.x, rectangle.bottom);
		hotspots[2] = Vector2D((rectangle.center.x - rectangle.extents.x / offset), rectangle.bottom);
	} else if (side == RectangleSide::SideTop) {
		//create 3 hotspots Left, Middle, Right
		hotspots[0] = Vector2D((rectangle.center.x + rectangle.extents.x / offset), rectangle.top);
		hotspots[1] = Vector2D(rectangle.center.x, rectangle.top);
		hotspots[2] = Vector2D((rectangle.center.x - rectangle.extents.x / offset), rectangle.top);
	} else if (side == RectangleSide::SideLeft) {
		//create 3 hotspots Top, Middle, Bottom
		hotspots[0] = Vector2D(rectangle.left, (rectangle.center.y + rectangle.extents.y / offset));
		hotspots[1] = Vector2D(rectangle.left, rectangle.center.y);
		hotspots[2] = Vector2D(rectangle.left, (rectangle.center.y - rectangle.extents.y / offset));
	} else if (side == RectangleSide::SideRight) {
		//create 3 hotspots Top, Middle, Bottom
		hotspots[0] = Vector2D(rectangle.right, (rectangle.center.y + rectangle.extents.y / offset));
		hotspots[1] = Vector2D(rectangle.right, rectangle.center.y);
		hotspots[2] = Vector2D(rectangle.right, (rectangle.center.y - rectangle.extents.y / offset));
	}

	//for each hotspot check if it is colliding
	for (int i = 0; i < 3; i++)
	{
		//if we are colliding at the point then set isCollidingOnSide to true
		if (IsCollidingAtPosition(hotspots[i].x, hotspots[i].y)) {
			isCollidingOnSide = true;
		}
	}

	return isCollidingOnSide;
}

bool ColliderTilemap::IsCollidingAtPosition(float x, float y) const
{
	//create a point in the normalized gridsystem that is the Tilemap
	Vector2D point = Vector2D(x , y);
	point = transform->GetInverseMatrix() * point;

	int x2 = static_cast<int>(point.x + 0.5f);
	int y2 = static_cast<int>(-point.y + 0.5f);

	int cellValue = map->GetCellValue(x2, y2);

	return (cellValue > 0);
}

// Move an object and set its velocity based on where it collided with the tilemap.
// Params:
//   objectRectangle = A bounding rectangle that encompasses the object.
//   objectTransform = Pointer to the object's transform component.
//   objectPhysics = Pointer to the object's physics component.
//   collisions = True/false values from map collision checks.
void ColliderTilemap::ResolveCollisions(const BoundingRectangle & objectRectangle, Transform * objectTransform, Physics * objectPhysics, const MapCollision & collisions) const
{	
	// Get the translation and velocity from the object and store them in variables
	Vector2D translation = objectTransform->GetTranslation();
	Vector2D velocity = objectPhysics->GetVelocity();

	// We need to determine how much to move the object
	float nudgeAmount;
	// We will do this by finding the center of the next tile, 
	// then finding the distance from the corresponding side to that position
	float nextTileCenter;

	//if the object is colliding on the bottom or top
	if (collisions.bottom || collisions.top)
	{
		//if the object is colliding on the bottom
		if (collisions.bottom)
		{
			// Find tile above the object's bottom side
			nextTileCenter = GetNextTileCenter(SideBottom, objectRectangle.bottom);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.bottom;
		}
		else
		{
			// Find tile below object's top side
			nextTileCenter = GetNextTileCenter(SideTop, objectRectangle.top);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.top;
		}
		
		// Nudge object up/down
		translation.y += nudgeAmount;
		// Stop velocity in y direction
		velocity.y = 0;
	}

	//if the object is colliding on the left or right
	if(collisions.left || collisions.right)
	{
		//if the object is colliding on the left
		if (collisions.left) {
			// Find tile to the right the object's left side
			nextTileCenter = GetNextTileCenter(SideLeft, objectRectangle.left);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.left;
		} else {
			// Find tile to the left the object's right side
			nextTileCenter = GetNextTileCenter(SideRight, objectRectangle.right);
			// Find distance to next tile center
			nudgeAmount = nextTileCenter - objectRectangle.right;
		}

		// Nudge object left/right
		translation.x += nudgeAmount;
		// Stop velocity in x direction
		velocity.x = 0;
	}

	// Modify object's actual translation and velocity
	objectTransform->SetTranslation(translation);
	objectPhysics->SetVelocity(velocity);
}


// Find the center of the closest tile on the x-axis or y-axis.
// Used for assisting in collision resolution on a particular side.
// Params:
//   side = Which side the collision is occurring on.
//   sidePosition = The x or y value of that side.
//   point = the objects position
// Returns:
//   The center of the closest tile to the given position on the given side.
float ColliderTilemap::GetNextTileCenter(RectangleSide side, float sidePosition) const
{
	Vector2D point;

	// Create a pointer/reference variable to store the result
	float* result;

	//if the object is colliding on the bottom or top
	if(side == SideBottom || side == SideTop)
	{
		// Use the given position for the y-axis
		// Since we only care about one axis, the other will be 0
		point.y = sidePosition;
		// Remember which part we want to modify
		result = &point.y;
	} else {
		// Use the given position for the x-axis
		// Since we only care about one axis, the other will be 0
		point.x = sidePosition;
		// Remember which part we want to modify
		result = &point.x;
	}

	// Transform the world space point into tile space
	point = transform->GetInverseMatrix() * point;
	// Flip the y-axis
	point.y = -point.y;
	// Move completely into cell
	point += Vector2D(0.5, 0.5);

	// Find the next tile index (higher or lower depending on the side)
	//if the object is colliding on the left or top
	if (side == SideLeft || side == SideTop) {
		*result = ceil(*result);
	} else {
		*result = floor(*result);
	}

	// Re-adjust object position
	point -= Vector2D(0.5, 0.5);
	// Flip the y-axis
	point.y = -point.y;
	// Transform point back into world space
	point = transform->GetMatrix() * point;

	return *result;
}

Vector2D ColliderTilemap::ConvertTileMapCordsToWorldCords(Vector2D inputCords) {
	Vector2D Output = Vector2D(inputCords.x, -inputCords.y);
	Output = transform->GetMatrix() * Output;

	return Vector2D(Output.x + 0.5f, Output.y + 0.5f);
}

Vector2D ColliderTilemap::ConvertWorldCordsToTileMapCords(Vector2D inputCords)
{
	// Transform the world space point into tile space
	Vector2D point = transform->GetInverseMatrix() * inputCords;
	// Flip the y-axis
	point.y = -point.y;
	// Move completely into cell
	point += Vector2D(0.5, 0.5);

	point.x = floor(point.x);
	point.y = floor(point.y);

	return point;
}