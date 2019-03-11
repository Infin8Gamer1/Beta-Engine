//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"
#include "Transform.h"
#include "GameObject.h"
#include <Parser.h>

Physics::Physics() : Component("Physics")
{
	// Velocity may be stored as a direction vector and speed scalar, instead.
	velocity = Vector2D(0,0);

	// Angular velocity, the speed and direction that the object rotates.
	angularVelocity = 0.0f;

	// Used when calculating acceleration due to forces.
	inverseMass = 1.0f;

	// The sum of all forces acting on an object
	forcesSum = Vector2D(0,0);

	// Acceleration = inverseMass * (sum of forces)
	acceleration = Vector2D(0,0);

	// Previous position.  May be used for resolving collisions.
	oldTranslation = Vector2D(0,0);

	// Transform component
	transform = nullptr;

	//the gravity that should get applyed
	gravity = Vector2D(0,0);
}

Component * Physics::Clone() const
{
	return new Physics();
}

void Physics::Deserialize(Parser & parser)
{
	parser.ReadVariable("acceleration", acceleration);

	parser.ReadVariable("velocity", velocity);

	parser.ReadVariable("angularVelocity", angularVelocity);

	float mass;
	parser.ReadVariable("mass", mass);
	SetMass(mass);

	parser.ReadVariable("gravity", gravity);
}

void Physics::Serialize(Parser & parser) const
{
	parser.WriteVariable("acceleration", acceleration);

	parser.WriteVariable("velocity", velocity);

	parser.WriteVariable("angularVelocity", angularVelocity);

	parser.WriteVariable("mass", 1 / inverseMass);

	parser.WriteVariable("gravity", gravity);
}

void Physics::Initialize()
{
	transform = GetOwner()->GetComponent<Transform>();
}

void Physics::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	AddForce(gravity);

	acceleration = forcesSum * inverseMass;
	//std::cout << "forceSum : " << forcesSum.x << ", " << forcesSum.y << std::endl;
	forcesSum = Vector2D(0, 0);
}

void Physics::FixedUpdate(float dt)
{
	velocity = velocity + acceleration * dt;

	oldTranslation = transform->GetTranslation();

	transform->SetTranslation(oldTranslation + velocity * dt);

	transform->SetRotation(transform->GetRotation() + angularVelocity * dt);
}

void Physics::SetVelocity(const Vector2D & _velocity)
{
	velocity = _velocity;
}

const Vector2D & Physics::GetVelocity() const
{
	return velocity;
}

void Physics::SetAngularVelocity(float _velocity)
{
	angularVelocity = _velocity;
}

float Physics::GetAngularVelocity() const
{
	return angularVelocity;
}

void Physics::SetMass(float mass)
{
	inverseMass = 1 / mass;
}

void Physics::AddForce(const Vector2D & force)
{
	forcesSum += force;
}

const Vector2D & Physics::GetAcceleration() const
{
	return acceleration;
}

const Vector2D & Physics::GetOldTranslation() const
{
	return oldTranslation;
}

void Physics::SetGravity(Vector2D _gravity)
{
	gravity = _gravity;
}

Vector2D Physics::GetGravity()
{
	return gravity;
}
