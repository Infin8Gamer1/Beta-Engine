//------------------------------------------------------------------------------
//
// File Name:	MeshHelper.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MeshHelper.h"
#include <MeshFactory.h>
#include <Vertex.h>

Mesh * CreateTriangleMesh(const Color & color0, const Color & color1, const Color & color2)
{
	MeshFactory::GetInstance().AddTriangle(Vertex(Vector2D(-0.5,0.5), color0), Vertex(Vector2D(-0.5, -0.5), color1), Vertex(Vector2D(0.5, 0), color2));
	return MeshFactory::GetInstance().EndCreate();
}

Mesh * CreateQuadMesh(const Vector2D & textureSize, const Vector2D & extents)
{
	Vertex p1 = Vertex(Vector2D(-0.5f * extents.x, -0.5f * extents.y), Vector2D(0 * textureSize.x, 1 * textureSize.y));
	Vertex p2 = Vertex(Vector2D(0.5f  * extents.x, -0.5f * extents.y), Vector2D(1 * textureSize.x, 1 * textureSize.y));
	Vertex p3 = Vertex(Vector2D(-0.5f * extents.x, 0.5f  * extents.y), Vector2D(0 * textureSize.x, 0 * textureSize.y));
	Vertex p4 = Vertex(Vector2D(0.5f  * extents.x, 0.5f  * extents.y), Vector2D(1 * textureSize.x, 0 * textureSize.y));

	MeshFactory::GetInstance().AddTriangle(p1, p2, p3);
	MeshFactory::GetInstance().AddTriangle(p2, p4, p3);
	return MeshFactory::GetInstance().EndCreate();
}
