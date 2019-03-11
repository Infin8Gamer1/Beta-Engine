//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Intersection2D.h"
#include <Vector2D.h>
#include <Transform.h>
#include <Physics.h>

bool PointCircleIntersection(const Vector2D & point, const Circle & circle)
{
	float distanceSq = point.DistanceSquared(circle.center);
	float radiusSq = circle.radius * circle.radius;

	if (distanceSq <= radiusSq) {
		return true;
	}

	return false;
}

bool PointRectangleIntersection(const Vector2D & point, const BoundingRectangle & rect)
{
	if ((point.x >= rect.left && point.x <= rect.right) && (point.y >= rect.bottom && point.y <= rect.top)) {
		return true;
	}

	return false;
}

bool CircleCircleIntersection(const Circle & circle1, const Circle & circle2)
{
	float distance = circle1.center.Distance(circle2.center);
	float radi = (circle1.radius) + (circle2.radius);

	if (distance <= radi) {
		return true;
	}

	return false;
}

bool RectangleRectangleIntersection(const BoundingRectangle & rect1, const BoundingRectangle & rect2)
{
	if (rect1.right < rect2.left) {
		return false;
	}
	
	if (rect1.bottom > rect2.top) {
		return false;
	}

	if (rect1.left > rect2.right) {
		return false;
	}

	if (rect1.top < rect2.bottom) {
		return false;
	}

	return true;
}

bool RectangleCircleIntersection(const BoundingRectangle & rect, const Circle & circle)
{
	Vector2D point = circle.center;

	if (point.x > rect.right) {
		point.x = rect.right;
	} else if (point.x < rect.left) {
		point.x = rect.left;
	}

	if (point.y > rect.top) {
		point.y = rect.top;
	} else if (point.y < rect.bottom) {
		point.y = rect.bottom;
	}

	return PointCircleIntersection(point, circle);
}

bool MovingPointLineIntersection(const LineSegment & staticLine, const LineSegment & movingPoint, Vector2D & intersection, float & t)
{
	// 1. Calculate the second object's change in translation during the current frame. Store this in your velocity variable.
	Vector2D Velocity = movingPoint.end - movingPoint.start;

	// 2. Return false if the second object is stationary or is moving parallel to the line segment. 
	// (HINT: Use dot product with the velocity and line's normal!)
	if (Velocity.DotProduct(staticLine.normal) == 0)
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////
	//								//
	// Now we calculate the time of intersection.			//
	// Get ready for lots of dot products!				//
	//								//
	// FIRST LINE - NORMAL EQ (static line)				//
	// NOTE: a = normal.x, b = normal.y, p = start			//
	// a * x + b * y = c						//
	// c = a * p.x + b * p.y = normal dot p				//
	// Thus, normal dot xy = normal dot p				//
	//								//
	// SECOND LINE - VECTOR EQ (moving point)			//
	// xy = start + (start - end) * t				//
	// From earlier, we know that start - end = velocity		//
	// Thus, xy = start + velocity * t				//
	//								//
	// COMBINED EQs							//
	// normal dot (start + velocity * t) = normal dot p		//
	// normal dot start + normal dot velocity * t = normal dot p	//
	//								//
	//////////////////////////////////////////////////////////////////

	// 3. Using the above information, solve for t.
	t = (staticLine.normal.DotProduct(staticLine.start) - staticLine.normal.DotProduct(movingPoint.start)) / staticLine.normal.DotProduct(Velocity);

	// 4. Check if intersection is between moving point start and end (if t is < 0 or > 1)
	// If not between start and end, return false
	if (t < 0 || t > 1)
	{
		return false;
	}

	// 5. Calculate the point of intersection using the start, velocity, and t.
	intersection = movingPoint.start + Velocity * t;

	// 6. Verify intersection point is on static segment (using static line direction as normal)
	// If intersection point is not between static line start and static line end return false
	float intersectionDotStart = (intersection - staticLine.start).DotProduct((staticLine.end - staticLine.start));
	float intersectionDotEnd = (intersection - staticLine.end).DotProduct((staticLine.start - staticLine.end));

	if (intersectionDotStart < 0 || intersectionDotEnd < 0)
	{
		return false;
	}

	// 7. Other possibilities have been eliminated, so
	// this must be an intersection. Return true!
	//std::cout << "We Collided at " << intersection << std::endl;
	return true;
}

void MovingPointLineReflection(Transform & transform, Physics & physics, const LineSegment & staticLine, const LineSegment & movingPoint, const Vector2D & intersection)
{
	// 1. Find correct position of object by reflecting its end point over the line.
	// (Use the ReflectPointOverLine function!)
	// Once found, set the object's translation to this reflected point.
	Vector2D reflectedPoint = ReflectPointOverLine(movingPoint.end, staticLine);
	transform.SetTranslation(reflectedPoint);

	// 2. Use the reflected position and intersection point to find the
	// direction of the reflected velocity. Make sure to normalize this vector!
	Vector2D reflectedVelocity = reflectedPoint - intersection;
	reflectedVelocity = reflectedVelocity.Normalized();

	// 3. Set the velocity of the object to this direction times
	// the magnitude of the current velocity.
	physics.SetVelocity(reflectedVelocity * physics.GetVelocity().Magnitude());

	// 4. Find the object's new rotation by using atan2 with the reflected direction.
	// Once found, set the rotation of the object to this angle.
	float rotation = atan2(reflectedVelocity.y, reflectedVelocity.x);
	transform.SetRotation(rotation);
}

Vector2D ReflectPointOverLine(const Vector2D & point, const LineSegment & line)
{
	// 1. Pretend everything is at the origin by subtracting one of the line's 
	// points from the point we are reflecting.
	Vector2D originPoint = point - line.end;

	//////////////////////////////////////////////////////////////////////////////////
	//										//
	// Equation for reflected point R is as follows, given point P,			//
	// and a line with normal n:							//
	//		R = P - 2 * projection of P onto n				//
	//										//
	// Equation for projected point J (projection of P onto n) is as follows:	//
	//										//
	//		J = (P dot n) / (magnitude(n) squared) * n			//
	//										//
	// However, since our normal is normalized (magnitude = 1), this simplifies	//
	// the projection (and hence the reflection) significantly:			//
	//										//
	//		J = (P dot n) * n						//
	//										//
	//////////////////////////////////////////////////////////////////////////////////

	// 2. Calculate the reflected point and store it in a variable.
	// Reflected point = P - 2 * (P dot n) * n
	Vector2D reflectedPoint = originPoint - 2 * (originPoint.DotProduct(line.normal) * line.normal);

	// 3. We need to remember to move things back away from the origin before returning.
	// Return the reflected point + line.start (or line.end, if you used that at the beginning)
	reflectedPoint = reflectedPoint + line.end;

	return reflectedPoint;
}

bool MovingCircleLineIntersection(const LineSegment & staticLine, const LineSegment & movingPoint, const float radius, Vector2D & intersection, float & t)
{
	UNREFERENCED_PARAMETER(staticLine);
	UNREFERENCED_PARAMETER(movingPoint);
	UNREFERENCED_PARAMETER(radius);
	UNREFERENCED_PARAMETER(intersection);
	UNREFERENCED_PARAMETER(t);
	return false;
}

bool PointIsBetweenLines(const Vector2D & point, const Vector2D & firstLine, const Vector2D & secondLine, const Vector2D & normal)
{
	UNREFERENCED_PARAMETER(point);
	UNREFERENCED_PARAMETER(firstLine);
	UNREFERENCED_PARAMETER(secondLine);
	UNREFERENCED_PARAMETER(normal);

	//////////////////////////////////////////////////////////////////////////////////
	//										//
	// To determine whether the point is between the two lines, we'll be		//
	// comparing distances of parallel lines going through each point		//
	// to the origin.								//
	//										//
	// Normally, distance from a point P to a line with point Q and normal n	//
	// is calculated as follows:							//
	//										//
	//		abs(n dot P - n dot Q) / magnitude(n)				//
	//										//
	// However, in this case, P is (0, 0), which simplifies the equation:		//
	//										//
	//		n dot Q / magnitude(n)						//
	//										//
	// Furthermore, N is normalized, further simplifying the equation:		//
	//										//
	//		n dot Q								//
	//										//
	//////////////////////////////////////////////////////////////////////////////////

	// 1. Calculate distances between the line through each point and the origin, 
	// store each result in its own variable.
	// (Distance from origin to line through any point = normal dot point)

	// 2. If the distance to the line through "point" is less than both the others,
	// it is not between them. Return false.

	// 3. If the distance to the line through "point" is greater than both the others,
	// it is not between them. Return false.

	// 4. All other cases eliminated, so it must be between them. Return true!
	
	return false;
}


