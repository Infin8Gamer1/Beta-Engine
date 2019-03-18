//------------------------------------------------------------------------------
//
// File Name:	MonkeyMovement.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MonkeyMovement.h"
#include <GameObject.h>
#include "Transform.h"
#include "Physics.h"
#include "Animation.h"
#include "Collider.h"
#include <Graphics.h>
#include <Input.h>
#include <Sprite.h>
#include <ResourceManager.h>

Behaviors::MonkeyMovement::MonkeyMovement() : Component("MonkeyMovement")
{
	// Components
	transform = nullptr;
	physics = nullptr;
	animation = nullptr;
	sprite = nullptr;
}

Component * Behaviors::MonkeyMovement::Clone() const
{
	return new MonkeyMovement();
}

void Behaviors::MonkeyMapCollisionHandler(GameObject & object, const MapCollision& collision)
{
	if (collision.bottom) {
		object.GetComponent<MonkeyMovement>()->onGround = true;
	}
	else if (collision.left) {
		object.GetComponent<MonkeyMovement>()->onWallLeft = true;
	}
	else if (collision.right) {
		object.GetComponent<MonkeyMovement>()->onWallRight = true;
	}
}

void Behaviors::MonkeyCollisionHandler(GameObject & object, GameObject & other)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(other);

	/*if (other.GetName() == "Collectable") {
		other.Destroy();
		static_cast<MonkeyMovement*>(object.GetComponent("MonkeyMovement"))->CoinsCollected += 1;
	}

	if (other.GetName() == "Hazard" || other.GetName() == "Enemy") {
		static_cast<MonkeyMovement*>(object.GetComponent("MonkeyMovement"))->Health -= 1;
	}*/
}

void Behaviors::MonkeyMovement::Initialize()
{
	onGround = false;

	//get Components
	transform = GetOwner()->GetComponent<Transform>();
	physics = GetOwner()->GetComponent<Physics>();
	animation = GetOwner()->GetComponent<Animation>();
	sprite = GetOwner()->GetComponent<Sprite>();

	//set the collision handler for the monkey
	GetOwner()->GetComponent<Collider>()->SetMapCollisionHandler(MonkeyMapCollisionHandler);
	GetOwner()->GetComponent<Collider>()->SetCollisionHandler(MonkeyCollisionHandler);
}

void Behaviors::MonkeyMovement::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	//call movement code
	MoveVertical();
	MoveHorizontal();

	//do animations
	if (!animation->IsRunning()) {
		float xVelo = physics->GetVelocity().x;

		if (onGround && abs(xVelo) >= 7) {
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("MonkeyWalk", true));

			if (xVelo >= 0) {
				animation->Play(0.1f, false);
				return;
			}
			if (xVelo <= -0) {
				animation->Play(0.1f, false, true);
				return;
			}
		} else if (onGround && abs(xVelo) < 7) {
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("MonkeyIdle", true));
			animation->Play(0.05f, false);
		} else {
			sprite->SetSpriteSource(ResourceManager::GetInstance().GetSpriteSource("MonkeyJump", true));
			animation->Play(0.05f, false);
		}
	}
}

void Behaviors::MonkeyMovement::MoveHorizontal() const
{
	//left
	if (Input::GetInstance().IsKeyDown('A')) {
		physics->AddForce(Vector2D(-1, 0) * strafeForce);
	}
	//right
	if (Input::GetInstance().IsKeyDown('D')) {
		physics->AddForce(Vector2D(1, 0) * strafeForce);
	}
}

void Behaviors::MonkeyMovement::MoveVertical()
{
	//jump
	if (Input::GetInstance().CheckReleased(' ') && onGround) {
		physics->SetVelocity(Vector2D(physics->GetVelocity().x, jumpForce));
		onGround = false;
		//onWallLeft = false;
		//onWallRight = false;
	}

	if (Input::GetInstance().CheckReleased(' ') && onWallLeft && !onGround && !onWallRight && WallJump) {
		physics->SetVelocity(Vector2D(physics->GetVelocity().x + jumpForce / 2, jumpForce/2));
		//onGround = false;
		onWallLeft = false;
		//onWallRight = false;
	} 
	
	if (Input::GetInstance().CheckReleased(' ') && onWallRight && !onGround && !onWallLeft && WallJump) {
		physics->SetVelocity(Vector2D(physics->GetVelocity().x - jumpForce / 2, jumpForce/2));
		//onGround = false;
		onWallLeft = false;
		//onWallRight = false;
	}

	if (physics->GetVelocity().y < 0) {
		onGround = false;
	}
	if (physics->GetVelocity().x < 0) {
		onWallLeft = false;
	}
	if (physics->GetVelocity().x > 0) {
		onWallRight = false;
	}
}


