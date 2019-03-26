//------------------------------------------------------------------------------
//
// File Name:	Collider.h
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

#include "Component.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
class GameObject;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum ColliderType
{
	ColliderTypePoint,
	ColliderTypeCircle,
	ColliderTypeRectangle,
	ColliderTypeTilemap,
	ColliderTypeLines,
} ColliderType;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Booleans determining collision state for each side of the object.
struct MapCollision
{
	MapCollision(bool bottom, bool top, bool left, bool right);
	bool bottom;
	bool top;
	bool left;
	bool right;
};

// Function pointer for collision event handlers.
// Params:
//   callingObject = The object whose handler is being called.
//   otherObject = The other object that the object collided with.
typedef void(*CollisionEventHandler)(GameObject& callingObject, GameObject& otherObject);

// Function pointer for tilemap collision event handlers.
// Params:
//   object = The object that collided with the map.
//   sides = Collision state for each side of the object.
typedef void(*MapCollisionEventHandler)(GameObject& object,
	const MapCollision& sides);

// Collider class - Detects collisions between objects
class Collider : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Allocate a new collider component.
	// Params:
	//   type = The type of collider (see the ColliderType enum).
	Collider(ColliderType type);

	~Collider();

	// Set component pointers.
	void Initialize() override;

	// Add Component Spific Vars to a Tweak Bar
	void AddVarsToTweakBar(TwBar* bar) override;

	// Draw collision shape
	virtual void Draw() = 0;

	// Check if two objects are colliding and send collision events.
	// Params:
	//	 other = Reference to the second collider component.
	void CheckCollision(const Collider& other);

	// Perform intersection test between two arbitrary colliders.
	// Params:
	//	 other = Reference to the second collider component.
	virtual bool IsCollidingWith(const Collider& other) const = 0;

	// Get the type of this component.
	ColliderType GetType() const;

	// Sets the collision handler function for the collider.
	// Params:
	//   handler = A pointer to the collision handler function.
	void SetCollisionHandler(CollisionEventHandler handler);

	// Sets the map collision handler function for the collider.
	// Params:
	//   handler = A pointer to the collision handler function.
	void SetMapCollisionHandler(MapCollisionEventHandler mapHandler);
	
	// Get the map collision handler function pointer.
	MapCollisionEventHandler GetMapCollisionHandler() const;

	// Save object data to file.
	// Params:
	//   parser = The parser object used to save the object's data.
	void BaseSerialize(Parser& parser) const;

	// Load object data from file
	// Params:
	//   parser = The parser object used to load the object's data.
	void BaseDeserialize(Parser & parser);

	void Disable();

	void Enable();

protected:
	//------------------------------------------------------------------------------
	// Protected Variables:
	//------------------------------------------------------------------------------

	// Component pointers
	Transform* transform;
	Physics* physics;

private:

	// C++ template to print vector container elements 
	template <typename T>
	std::string printVector(const std::vector<T>& v) const
	{
		std::string os;
		for (int i = 0; i < v.size(); ++i) {
			os = os + v[i] + ", ";
		}

		os = os.substr(0, os.find_last_of(","));

		return os;
	}

	std::vector<std::string> explodeString(const std::string& str, const char& ch);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The type of collider used by this component.
	ColliderType type;

	// Function pointer for collision handler
	CollisionEventHandler handler;
	
	// Function pointer for tilemap collision handling
	MapCollisionEventHandler mapHandler;

	bool enabled;

	// The Group That This Collider Belongs To
	std::string CollisionGroup;

	// A List of strings that identify Collision Flags
	std::vector<std::string> CollisionFlags;

	TwBar* bar;
};

//------------------------------------------------------------------------------
