//------------------------------------------------------------------------------
//
// File Name:	Tilemap.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Tilemap.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Parser.h>

Tilemap::Tilemap(unsigned _numColumns, unsigned _numRows, int ** _data)
{
	// Dimensions of the map
	numColumns = _numColumns;
	numRows = _numRows;

	// The map data (a 2D array)
	data = _data;
}

Tilemap::Tilemap(std::string _name)
{
	name = _name;

	numColumns = 0;
	numRows = 0;

	data = nullptr;
}

Tilemap::~Tilemap()
{
	// Deleting the 2D array of tiles
	// delete in the opposite order of creation
	for (unsigned r = 0; r < numColumns; ++r)
	{
		delete[] data[r]; // delete each allocated row with array delete
		data[r] = nullptr; // safely set to nullptr
	}
	// Use array delete, since array new was used in allocation
	delete[] data;
	data = nullptr; // safely set to nullptr.
}

unsigned Tilemap::GetWidth() const
{
	return numColumns;
}

unsigned Tilemap::GetHeight() const
{
	return numRows;
}

int Tilemap::GetCellValue(unsigned column, unsigned row) const
{
	//if given row or column is outside of the array then just return -1
	if ((column >= numColumns || column < 0) || (row >= numRows || row < 0)) {
		return -1;
	}
	//other wise return the correct value
	return data[column][row];
}

Vector2D Tilemap::SetCellValue(int column, int row, int newValue) {
	//if given row or column is outside of the array then just return
	if ((column >= numColumns || column < 0) || (row >= numRows || row < 0)) {
		int colL = 0;
		int colR = 0;
		int rowT = 0;
		int rowB = 0;

		if (column < 0)
		{
			colL = -column;

			if (colL < 0) {
				colL = 0;
			}
		}
		else
		{
			colR = column - (numColumns - 1);

			if (colR < 0) {
				colR = 0;
			}
		}

		if (row < 0)
		{
			rowT = -row;

			if (rowT < 0) {
				rowT = 0;
			}
		}
		else
		{
			rowB = row - (numRows - 1);

			if (rowB < 0) {
				rowB = 0;
			}
		}

		Resize(colL, colR, rowT, rowB);
		
		data[column + colL][row + rowT] = newValue;
		return Vector2D((float)-colL, (float)rowT);
	}

	data[column][row] = newValue;

	return Vector2D();

	/*//loop through each value in the 2D array of tiles and print out its value
	for (unsigned r = 0; r < GetHeight(); r++)
	{
		for (unsigned c = 0; c < GetWidth(); c++)
		{
			std::cout << GetCellValue(c, r) << " ";
		}
		std::cout << std::endl;
	}*/
}

void Tilemap::Deserialize(Parser & parser)
{
	parser.ReadSkip(name);
	parser.ReadSkip("{");

	parser.ReadVariable("width", numColumns);
	parser.ReadVariable("height", numRows);

	parser.Read2DArrayVariable("data", data, numColumns, numRows);

	parser.ReadSkip("}");
}

void Tilemap::Serialize(Parser & parser) const
{
	parser.WriteValue(name);

	parser.BeginScope();

	parser.WriteVariable("width", numColumns);
	parser.WriteVariable("height", numRows);

	parser.Write2DArrayVariable("data", data, numColumns, numRows);

	parser.EndScope();
}

void Tilemap::Print()
{
	//print out data from tilemap to check if it was read sucessfully
	std::cout << "Height : " << GetHeight() << std::endl;
	std::cout << "Width : " << GetWidth() << std::endl;
	
	//loop through each value in the 2D array of tiles and print out its value
	for (unsigned r = 0; r < GetHeight(); r++)
	{
		for (unsigned c = 0; c < GetWidth(); c++)
		{
			std::cout << GetCellValue(c, r) << " ";
		}
		std::cout << std::endl;
	}
}

void Tilemap::Resize(int columnLeft, int columnRight, int rowTop, int rowBottom)
{
	int columns = numColumns + columnLeft + columnRight;
	int rows = numRows + rowTop + rowBottom;

	//make a new 2D int array
	int** temp = new int *[columns];
	for (int r = 0; r < columns; ++r)
	{
		temp[r] = new int[rows];
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			temp[j][i] = 0;
		}
	}

	for (unsigned i = 0; i < numRows; i++)
	{
		for (unsigned j = 0; j < numColumns; j++)
		{
			int value = data[j][i];

			int x = j + columnLeft;
			int y = i + rowTop;

			temp[x][y] = value;
		}
	}

	// Deleting the 2D array of ints
	// delete in the opposite order of creation
	for (unsigned r = 0; r < numColumns; ++r)
	{
		delete[] data[r]; // delete each allocated row with array delete
		data[r] = nullptr; // safely set to nullptr
	}
	// Use array delete, since array new was used in allocation
	delete[] data;
	data = nullptr; // safely set to nullptr.

	data = temp;
	numColumns = columns;
	numRows = rows;

	/*// Deleting the 2D array of ints
	// delete in the opposite order of creation
	for (unsigned r = 0; r < columns; ++r)
	{
		delete[] temp[r]; // delete each allocated row with array delete
		temp[r] = nullptr; // safely set to nullptr
	}
	// Use array delete, since array new was used in allocation
	delete[] temp;
	temp = nullptr; // safely set to nullptr.*/

	//Print();
}

std::string Tilemap::GetName() const
{
	return name;
}

void Tilemap::setName(std::string _name)
{
	name = _name;
}
