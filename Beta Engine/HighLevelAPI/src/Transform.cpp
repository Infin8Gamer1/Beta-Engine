//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include <Parser.h>
#include <sstream>
#include <Engine.h>
#include <SpaceManager.h>

Transform::Transform(float x, float y) : Component("Transform")
{
	translation = Vector2D(x, y);
	scale = Vector2D(50,50);
	rotation = 0.0f;

	matrix = CS230::Matrix2D();

	isDirty = true;

	bar = nullptr;
}

Transform::Transform(Vector2D _translation, Vector2D _scale, float _rotation) : Component("Transform")
{
	translation = _translation;
	scale = _scale;
	rotation = _rotation;

	matrix = CS230::Matrix2D();

	isDirty = true;

	bar = nullptr;
}

Transform::~Transform()
{
	if (bar != nullptr)
	{
		TwRemoveVar(bar, "Position");
		TwRemoveVar(bar, "Rotation");
		TwRemoveVar(bar, "Scale");
	}
}

Component * Transform::Clone() const
{
	return new Transform(*this);
}

void Transform::Deserialize(Parser & parser)
{
	//get translation
	Vector2D _translation;
	parser.ReadVariable("translation", _translation);

	SetTranslation(_translation);

	//get rotation
	float _rotation;
	parser.ReadVariable("rotation", _rotation);

	SetRotation(_rotation);

	//get scale
	Vector2D _scale;
	parser.ReadVariable("scale", _scale);

	SetScale(_scale);
}

void Transform::Serialize(Parser & parser) const
{
	//set translation
	parser.WriteVariable("translation", GetTranslation());

	//set rotation
	parser.WriteVariable("rotation", GetRotation());

	//set scale
	parser.WriteVariable("scale", GetScale());
}

void Transform::AddVarsToTweakBar(TwBar * bar_)
{
	bar = bar_;

	Component::AddVarsToTweakBar(bar);
	std::string params = " group='" + GetName() + "' ";

	TwAddVarRW(bar, "Position", Engine::GetInstance().GetModule<SpaceManager>()->GetVector2DTwType(), (void*)&translation, params.c_str());
	TwAddVarRW(bar, "Rotation", TW_TYPE_FLOAT, &rotation, std::string(params + "precision=3 step=" + std::to_string(15.0f * M_PI / 180.0f) + " min=0 max=" + std::to_string(2*M_PI) + " ").c_str());
	TwAddVarRW(bar, "Scale", Engine::GetInstance().GetModule<SpaceManager>()->GetVector2DTwType(), (void*)&scale, params.c_str());
}

void Transform::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	if (bar != nullptr)
	{
		if (translation.x != previousTranslation.x || translation.y != previousTranslation.y)
		{
			isDirty = true;
		}

		previousTranslation = translation;

		if (rotation != previousRotation)
		{
			isDirty = true;
		}

		previousRotation = rotation;

		if (scale.x != previousScale.x || scale.y != previousScale.y)
		{
			isDirty = true;
		}

		previousScale = scale;
	}	
}

const CS230::Matrix2D & Transform::GetMatrix()
{
	CalculateMatrices();

	return matrix;
}

const CS230::Matrix2D & Transform::GetInverseMatrix()
{
	CalculateMatrices();

	return inverseMatrix;
}

void Transform::SetTranslation(const Vector2D & _translation)
{
	translation = _translation;

	isDirty = true;
}

const Vector2D & Transform::GetTranslation() const
{
	return translation;
}

void Transform::SetRotation(float _rotation)
{
	rotation = _rotation;

	isDirty = true;
}

void Transform::LookAt(Vector2D _target)
{
	Vector2D dif = _target - translation;

	dif.Normalized();

	SetRotation(atan2(dif.y, dif.x));
}

float Transform::GetRotation() const
{
	return rotation;
}

void Transform::SetScale(const Vector2D & _scale)
{
	scale = _scale;

	isDirty = true;
}

const Vector2D & Transform::GetScale() const
{
	return scale;
}

void Transform::CalculateMatrices()
{
	if (isDirty) {
		CS230::Matrix2D translationMatrix = CS230::Matrix2D().TranslationMatrix(translation.x, translation.y);
		CS230::Matrix2D rotationMatrix = CS230::Matrix2D().RotationMatrixRadians(rotation);
		CS230::Matrix2D scalingMatrix = CS230::Matrix2D().ScalingMatrix(scale.x, scale.y);

		matrix = translationMatrix * rotationMatrix * scalingMatrix;

		CS230::Matrix2D inverseTranslationMatrix = CS230::Matrix2D().TranslationMatrix(-translation.x, -translation.y);
		CS230::Matrix2D inverseRotationMatrix = CS230::Matrix2D().RotationMatrixRadians(-rotation);
		CS230::Matrix2D inverseScalingMatrix = CS230::Matrix2D().ScalingMatrix(1 / scale.x, 1 / scale.y);

		inverseMatrix = inverseScalingMatrix * inverseRotationMatrix * inverseTranslationMatrix;

		isDirty = false;
	}
}
