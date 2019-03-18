//------------------------------------------------------------------------------
//
// File Name:	Matrix2DStudent.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2DStudent.h"

namespace CS230 {

	Matrix2D::Matrix2D()
	{
		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				m[row][column] = 0.0f;
			}
		}
	}

	Matrix2D Matrix2D::IdentityMatrix()
	{
		Matrix2D outMatrix = Matrix2D();

		outMatrix.m[0][0] = 1.0f;
		outMatrix.m[1][1] = 1.0f;
		outMatrix.m[2][2] = 1.0f;

		return outMatrix;
	}

	Matrix2D Matrix2D::Transposed() const
	{
		Matrix2D outMatrix = Matrix2D();
		
		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				outMatrix.m[column][row] = m[row][column];
			}
		}

		return outMatrix;
	}

	Matrix2D Matrix2D::TranslationMatrix(float x, float y)
	{
		Matrix2D outMatrix = Matrix2D().IdentityMatrix();

		outMatrix.m[0][2] = x;
		outMatrix.m[1][2] = y;

		return outMatrix;
	}

	Matrix2D Matrix2D::ScalingMatrix(float x, float y)
	{
		Matrix2D outMatrix = Matrix2D().IdentityMatrix();

		outMatrix.m[0][0] = x;
		outMatrix.m[1][1] = y;

		return outMatrix;
	}

	Matrix2D Matrix2D::RotationMatrixDegrees(float angle)
	{
		Matrix2D outMatrix = Matrix2D().IdentityMatrix();

		angle = angle * 3.14159265358979323846f / 180.0f;

		outMatrix.m[0][0] = cos(angle);
		outMatrix.m[0][1] = -sin(angle);
		outMatrix.m[1][0] = sin(angle);
		outMatrix.m[1][1] = cos(angle);
		
		return outMatrix;
	}

	Matrix2D Matrix2D::RotationMatrixRadians(float angle)
	{
		Matrix2D outMatrix = Matrix2D().IdentityMatrix();

		outMatrix.m[0][0] = cos(angle);
		outMatrix.m[0][1] = -sin(angle);
		outMatrix.m[1][0] = sin(angle);
		outMatrix.m[1][1] = cos(angle);

		return outMatrix;
	}

	Matrix2D Matrix2D::operator*(const Matrix2D & other) const
	{
		Matrix2D outMatrix = Matrix2D();

		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				outMatrix.m[row][column] = RowColumnMultiply(other, row, column);
			}
		}

		return outMatrix;
	}

	Matrix2D & Matrix2D::operator*=(const Matrix2D & other)
	{
		Matrix2D outMatrix = *this * other;
		*this = outMatrix;

		return *this;
	}

	Vector2D Matrix2D::operator*(const Vector2D & vec) const
	{
		Vector2D outVector = Vector2D();

		outVector.x = (m[0][0] * vec.x) + (m[0][1] * vec.y) + (m[0][2]);
		outVector.y = (m[1][0] * vec.x) + (m[1][1] * vec.y) + (m[1][2]);

		return outVector;
	}

	float Matrix2D::RowColumnMultiply(const Matrix2D & other, unsigned row, unsigned col) const
	{
		return (m[row][0] * other.m[0][col]) + (m[row][1] * other.m[1][col]) + (m[row][2] * other.m[2][col]);
	}

}