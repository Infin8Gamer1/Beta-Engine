//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"
#include "Matrix2DStudent.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Transform.h.
// You are free to change the contents of this structure as long as you do not
//   change the public method declarations declared in the header.
class Transform : public Component
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Transform constructor.
	// Params:
	//	 x = Initial world position on the x-axis.
	//	 y = Initial world position on the y-axis.
	Transform(float x, float y);

	// Transform constructor.
	// Params:
	//	 translation = World position of the object.
	//   scale		 = Width and height of the object.
	//   rotation	 = Rotation of the object about the z-axis.
	Transform(Vector2D translation = Vector2D(), Vector2D scale = Vector2D(1,1), float rotation = 0);

	~Transform();

	// Clone the transform, returning a dynamically allocated copy.
	Component* Clone() const override;

	/*!
	 * Loads object data from a file.
	 * 
	 * \param parser
	 */
	void Deserialize(Parser& parser) override;

	/*!
	 * Saves Object data to a file.
	 * 
	 * \param parser
	 */
	void Serialize(Parser& parser) const override;

	// Add Component Spific Vars to a Tweak Bar
	void AddVarsToTweakBar(TwBar* bar) override;

	void Update(float dt) override;

	// Get the transform matrix, based upon translation, rotation and scale settings.
	// Returns:
	//	 A reference to the component's matrix.
	const CS230::Matrix2D& GetMatrix();

	// Get the inverse of the transform matrix.
	// Returns:
	//	 A reference to the component's inverse matrix.
	const CS230::Matrix2D& GetInverseMatrix();

	// Set the translation of a transform component.
	// Params:
	//	 translation = Reference to a translation vector.
	void SetTranslation(const Vector2D& translation);

	// Get the translation of a transform component.
	// Returns:
	//	 A reference to the component's translation structure.
	const Vector2D& GetTranslation() const;

	// Set the rotation of a transform component.
	// Params:
	//	 rotation = The rotation value (in radians).
	void SetRotation(float rotation);

	// Set the rotation of a transform component. So that it points twords the target
	// Params:
	//   target = the position of the target to look at.
	void LookAt(Vector2D target);

	// Get the rotation value of a transform component.
	// Returns:
	//	 The component's rotation value (in radians).
	float GetRotation() const;

	// Set the scale of a transform component.
	// Params:
	//	 translation = Reference to a scale vector.
	void SetScale(const Vector2D& scale);

	// Get the scale of a transform component.
	// Returns:
	//	 A reference to the component's scale structure.
	const Vector2D& GetScale() const;

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Calculates the transform matrix and its inverse using 
	// translation, rotation, and scale.
	void CalculateMatrices();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The translation (or world position) of a game object.
	Vector2D translation;
	// The Previous Translation of the game object. (only gets updated if there is a TwBar)
	Vector2D previousTranslation;

	// The rotation (or orientation) of a game object.
	float rotation;
	// The previous rotation of the game object. (only gets updated if there is a TwBar)
	float previousRotation;

	// The scale (or size) of a game object.
	Vector2D scale;
	// The previous Scale of the game object. (only gets updated if there is a TwBar)
	Vector2D previousScale;

	// The transformation matrix resulting from multiplying the 
	// translation, rotation, and scale matrices.
	CS230::Matrix2D	matrix;
	CS230::Matrix2D inverseMatrix;

	// True if the transformation matrix needs to be recalculated.
	bool isDirty;

	TwBar* bar;
};

//------------------------------------------------------------------------------
