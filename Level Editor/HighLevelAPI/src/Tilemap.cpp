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

Tilemap::Tilemap(unsigned _numColumns, unsigned _numRows, int ** _data)
{
	// Dimensions of the map
	numColumns = _numColumns;
	numRows = _numRows;

	// The map data (a 2D array)
	data = _data;
		
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

Tilemap * Tilemap::CreateTilemapFromFile(const std::string & filename)
{
	std::ifstream infile(filename);
	if (!infile.is_open()) 
	{ 
		std::cout << "Can't open file '" + filename + "'." << std::endl;
		return nullptr;
	}
	
	int rows = -1;
	int columns = -1;

	ReadIntegerVariable(infile, "width", columns);
	ReadIntegerVariable(infile, "height", rows);

	int** data = ReadArrayVariable(infile, "data", columns, rows);

	Tilemap* output = new Tilemap(columns, rows, data);

	output->setName(filename);

	infile.close();

	//print out data from tilemap to check if it was read sucessfully
	std::cout << "Height : " << output->GetHeight() << std::endl;
	std::cout << "Width : " << output->GetWidth() << std::endl;
	
	//loop through each value in the 2D array of tiles and print out its value
	for (unsigned r = 0; r < output->GetHeight(); r++)
	{
		for (unsigned c = 0; c < output->GetWidth(); c++)
		{
			std::cout << output->GetCellValue(c, r) << "   ";
		}
		std::cout << std::endl;
	}
	
	return output;
}

std::string Tilemap::GetName() const
{
	return name;
}

bool Tilemap::ReadIntegerVariable(std::ifstream & file, const std::string & name, int & variable)
{
	std::string str;
	//read name
	file >> str;
	//if correct name then return true and put stuff in variable
	if (str == name) {
		file >> variable;
		return true;
	}
	//we didn't find it and we are failures!
	return false;
}

int ** Tilemap::ReadArrayVariable(std::ifstream & file, const std::string & name, unsigned columns, unsigned rows)
{

	std::string str;
	//read name
	file >> str;

	if (str != name) {
		return nullptr;
	}

	//make a new 2D int array
	int ** data = new int *[columns];
	for (unsigned r = 0; r < columns; ++r)
	{
		//warning creates memory leak!
		//TODO: FIX MEMORY LEAK FROM THIS!
		data[r] = new int[rows];
	}

	//loop through each value in the 2D array of tiles and set each one to the correct one here
	for (unsigned r = 0; r < rows; r++)
	{
		for (unsigned c = 0; c < columns; c++)
		{
			//read the next int
			int value;
			file >> value;
			//set that value in output
			data[c][r] = value;
		}
	}

	return data;
}

void Tilemap::setName(std::string _name)
{
	name = _name;
}
