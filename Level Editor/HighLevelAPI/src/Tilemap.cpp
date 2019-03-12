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

void Tilemap::SetCellValue(int column, int row, int newValue) const {
	//if given row or column is outside of the array then just return
	if ((column >= numColumns || column < 0) || (row >= numRows || row < 0)) {
		return;
	}

	data[column][row] = newValue;

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

std::string Tilemap::GetName() const
{
	return name;
}

void Tilemap::setName(std::string _name)
{
	name = _name;
}
