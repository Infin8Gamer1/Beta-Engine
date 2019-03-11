//------------------------------------------------------------------------------
//
// File Name:	ColliderRectangle.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderRectangle.h"
#include "ColliderPoint.h"
#include "ColliderCircle.h"
#include "GameObject.h"
#include "Transform.h"
#include "Intersection2D.h"
#include <DebugDraw.h>
#include <Graphics.h>
#include <Parser.h>

ColliderRectangle::ColliderRectangle(const Vector2D & _extents) : Collider(ColliderType::ColliderTypeRectangle)
{
	extents = _extents;
}

Component * ColliderRectangle::Clone() const
{
	return new ColliderRectangle(extents);
}

void ColliderRectangle::Deserialize(Parser & parser)
{
	BaseDeserialize(parser);
	parser.ReadVar(extents);
}

void ColliderRectangle::Serialize(Parser & parser) const
{
	BaseSerialize(parser);
	parser.WriteVar(extents);
}

void ColliderRectangle::Draw()
{
	DebugDraw::GetInstance().AddRectangle(transform->GetTranslation(), extents, Graphics::GetInstance().GetCurrentCamera(), Color(0,1,0,1));
}

const Vector2D & ColliderRectangle::GetExtents() const
{
	return extents;
}

void ColliderRectangle::SetExtents(const Vector2D & _extents)
{
	extents = _extents;
}

bool ColliderRectangle::IsCollidingWith(const Collider & other) const
{
	//if the other collider is a point then do things
	if (other.GetType() == ColliderType::ColliderTypePoint) {
		Transform* pointTransform = other.GetOwner()->GetComponent<Transform>();
		return PointRectangleIntersection(pointTransform->GetTranslation(), BoundingRectangle(transform->GetTranslation(), GetExtents()));
	}

	//if the other collider is a rectangle then test if they collided
	if (other.GetType() == ColliderType::ColliderTypeRectangle) {
		const ColliderRectangle& rect = static_cast<const ColliderRectangle&>(other);
		Transform* rectTransform = other.GetOwner()->GetComponent<Transform>();
		return RectangleRectangleIntersection(BoundingRectangle(rectTransform->GetTranslation(), rect.GetExtents()), BoundingRectangle(transform->GetTranslation(), GetExtents()));
	}

	//if the other collider is circle then check if they collided
	if (other.GetType() == ColliderType::ColliderTypeCircle) {
		const ColliderCircle& circ = static_cast<const ColliderCircle&>(other);
		Transform* circTransform = other.GetOwner()->GetComponent<Transform>();
		return RectangleCircleIntersection(BoundingRectangle(transform->GetTranslation(), GetExtents()), Circle(circTransform->GetTranslation(), circ.GetRadius()));
	}

	//other wise they didn't collide
	return false;
}
