#include "stdafx.h"
#include "ColliderLine.h"
#include <Graphics.h>
#include <DebugDraw.h>
#include <Physics.h>
#include <GameObject.h>
#include <Transform.h>
#include <ColliderPoint.h>
#include <ColliderCircle.h>
#include <Parser.h>

ColliderLine::ColliderLine(bool _reflection) : Collider(ColliderType::ColliderTypeLines)
{
	reflection = _reflection;
	includeScale = true;
}

Component * ColliderLine::Clone() const
{
	return new ColliderLine(reflection);
}

void ColliderLine::Draw()
{
	for (unsigned i = 0; i < lineSegments.size(); i++)
	{
		DebugDraw::GetInstance().AddLineToStrip(GetLineWithTransform(i).start, GetLineWithTransform(i).end, Color(0, 1, 0, 1));
		Vector2D midpoint = Vector2D((GetLineWithTransform(i).start.x + GetLineWithTransform(i).end.x) / 2, (GetLineWithTransform(i).start.y + GetLineWithTransform(i).end.y) / 2);
		DebugDraw::GetInstance().AddLineToStrip(midpoint, (GetLineWithTransform(i).normal * 20) + midpoint, Color(1, 0, 0, 1));
	}

	DebugDraw::GetInstance().EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
}

void ColliderLine::AddLineSegment(const Vector2D & p0, const Vector2D & p1)
{
	lineSegments.push_back(LineSegment(p0, p1));
}

bool ColliderLine::IsCollidingWith(const Collider & other) const
{
	LineSegment othersLine = LineSegment(other.GetOwner()->GetComponent<Physics>()->GetOldTranslation(), other.GetOwner()->GetComponent<Transform>()->GetTranslation());

	switch (other.GetType())
	{
	case ColliderType::ColliderTypeCircle:
	case ColliderType::ColliderTypePoint:

		for (unsigned i = 0; i < lineSegments.size(); i++)
		{
			LineSegment myLine = LineSegment(GetLineWithTransform(i).start, GetLineWithTransform(i).end);
			Vector2D intersection = Vector2D(0, 0);
			float t = 0;

			bool intersected = MovingPointLineIntersection(myLine, othersLine, intersection, t);

			//check if we intersected this frame
			if ((t < 1 || t > 0) && intersected) {
				if (reflection) {
					MovingPointLineReflection(*other.GetOwner()->GetComponent<Transform>(), *other.GetOwner()->GetComponent<Physics>(), myLine, othersLine, intersection);
				}
				return true;
			}
		}

	default:
		return false;
	}
}

void ColliderLine::Serialize(Parser & parser) const
{
	BaseSerialize(parser);

	parser.WriteVariable("LineTypeRelitive", includeScale);
	parser.WriteVariable("Reflection", reflection);

	parser.WriteVariable("NumberOfLines", lineSegments.size());
	for (size_t i = 0; i < lineSegments.size(); i++)
	{
		parser.WriteVariable("Line", lineSegments[i]);
	}
}

void ColliderLine::Deserialize(Parser & parser)
{
	BaseDeserialize(parser);

	parser.ReadVariable("LineTypeRelitive", includeScale);
	parser.ReadVariable("Reflection", reflection);

	unsigned numLines;
	parser.ReadVariable("NumberOfLines", numLines);

	for (unsigned i = 0; i < numLines; i++)
	{
		LineSegment line;
		parser.ReadVariable("Line", line);

		lineSegments.push_back(line);
	}
}

LineSegment ColliderLine::GetLineWithTransform(unsigned index) const
{
	Vector2D startPos = (lineSegments[index].start + transform->GetTranslation());

	Vector2D endPos = (lineSegments[index].end + transform->GetTranslation());
	
	if (includeScale)
	{
		Vector2D scale = transform->GetScale();

		startPos = Vector2D(startPos.x * scale.x, startPos.y * scale.y);
		endPos = Vector2D(endPos.x * scale.x, endPos.y * scale.y);
	}

	return LineSegment(startPos, endPos);
}
