//------------------------------------------------------------------------------
//
// File Name:	Parser.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Parser.h"

ParseException::ParseException(const std::string & fileName, const std::string & errorDetails) 
	: std::exception(("Error reading file " + fileName + ". " + errorDetails).c_str())
{
}

Parser::Parser(const std::string & _filename, std::ios_base::openmode _mode) : stream(_filename, _mode)
{	
	filename = _filename;
	mode = _mode;
	indentLevel = 0;
}

Parser::~Parser()
{
	stream.close();
}

void Parser::BeginScope()
{
	CheckFileOpen();

	for (unsigned i = 0; i < indentLevel; i++)
	{
		stream << tab;
	}

	stream << "{" << std::endl;

	indentLevel += 1;
}

void Parser::EndScope()
{
	CheckFileOpen();

	indentLevel -= 1;

	for (unsigned i = 0; i < indentLevel; i++)
	{
		stream << tab;
	}

	stream << "}" << std::endl;
}

void Parser::ReadSkip(const std::string & text)
{
	CheckFileOpen();

	//Create a string variable and use the input operator to place the next word in the stream into it.
	std::string word;
	stream >> word;

	//If the contents of the string don't match the name parameter, throw a ParseException with the file name and a message about how a variable with the given name could not be found.
	if (word != text) {
		throw ParseException(filename, "the text " + text + "didn't match " + word + ". ERROR 2");
	}
}

std::string Parser::ReadSkipComponent()
{
	CheckFileOpen();

	std::string readValue = "";

	//create a var to keep track of how many open brackets ({) have been read
	unsigned levels = 0;

	//before we can start the loop we need to see if the next word is ({) if it is then we will start the loop
	std::string wordA;
	stream >> wordA;

	if (wordA == "{") {
		levels += 1;

		readValue += wordA + " ";
	}

	while (levels > 0)
	{
		//read each word if it is { then add one to the counter otherwise if it is } remove one from the counter
		//continue until the counter <= 0

		std::string word;
		stream >> word;

		if (word == "{") {
			levels += 1;
		}
		else if (word == "}") {
			levels -= 1;
		}

		readValue += word + " ";
	}

	return readValue;
}

std::string Parser::ReadLine()
{
	CheckFileOpen();

	std::string line;

	stream >> line;

	return line;
}

void Parser::CheckFileOpen()
{
	if (!stream.is_open()) {
		throw ParseException(filename, "Filestream isn't open! ERROR 3");
	}
}

// Reads the value of a variable with the given name from the currently open file.
void Parser::Write2DArrayVariable(const std::string& name, int** variable, const int columns, const int rows) {
	CheckFileOpen();

	for (unsigned i = 0; i < indentLevel; i++)
	{
		stream << tab;
	}

	stream << name << " : ";

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			stream << variable[j][i] << " ";
		}
	}

	stream << std::endl;
}

// Reads the value of a variable with the given name from the currently open file.
void Parser::Read2DArrayVariable(const std::string& name, int**& variable, int columns, int rows) {
	CheckFileOpen();

	//Create a string variable and use the input operator (>>) to place the next word in the stream into it.
	std::string word;
	stream >> word;

	//If the contents of the string don't match the name parameter, throw a ParseException, passing it the name and filename.
	if (word != name) {
		throw ParseException(filename, "the name " + name + "didn't match " + word + ". ERROR 1");
	}

	//skip forward in the stream to after the next colon character
	ReadSkip(":");

	//make a new 2D int array
	variable = new int *[columns];
	for (int r = 0; r < columns; ++r)
	{
		variable[r] = new int[rows];
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			stream >> variable[j][i];
		}
	}
}
