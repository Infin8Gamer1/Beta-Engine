//------------------------------------------------------------------------------
//
// File Name:	Parser.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <exception>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//#define WRITE_VARIABLE(var) WriteVariable(#var, (var))

// Exception class for problems encountered while reading/writing from files
class ParseException : public std::exception
{
public:
	ParseException(const std::string& fileName, const std::string& errorDetails);
};

// Parser class - reads/writes data object data to/from files
// You are free to change the contents of this structure as long as you do not
// modify the public interface (function prototypes) declared in the header.
class Parser
{
public:
	// Opens a file for loading.
	// Returns:
	//   True if load was successful, false otherwise.
	Parser(const std::string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

	// Closes the currently open file.
	~Parser();

	// Begins a new scope when writing to a file.
	// Outputs a curly brace and increases the tab count.
	void BeginScope();

	// Ends the current scope when writing to a file.
	// Outputs an end curly brace and decreases tab count.
	void EndScope();

#define WriteVar(var) WriteVariable(#var, var)

	// Writes a variable name and value to the currently open file.
	template<typename T>
	void WriteVariable(const std::string& name, const T& variable) {
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; i++)
		{
			stream << tab;
		}

		stream << name << " : " << variable << std::endl;
	}

	// Writes a value to the currently open file.
	template<typename T>
	void WriteValue(const T& value) {
		CheckFileOpen();

		for (unsigned i = 0; i < indentLevel; i++)
		{
			stream << tab;
		}

		stream << value << std::endl;
	}

	void Write2DArrayVariable(const std::string& name, int** variable, const int columns, const int rows);

#define ReadVar(var) ReadVariable(#var, var)

	// Reads the value of a variable with the given name from the currently open file.
	template<class T>
	void ReadVariable(const std::string& name, T& variable) {
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

		//place the next item in the stream into variable
		stream >> variable;
	}

	// Reads the value of a variable with the given name from the currently open file.
	template <>
	void ReadVariable<std::string>(const std::string& name, std::string& variable) {
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

		//place the next item in the stream into variable
		char var[256];
		stream.getline(var, 256);

		variable = var;

		size_t strBegin = variable.find_first_not_of(" ");
		size_t strEnd = variable.find_last_not_of(" ");

		variable = variable.substr(strBegin, strEnd);
	}

	void Read2DArrayVariable(const std::string& name, int**& variable, int columns, int rows);

	// Reads the next value from the currently open file.
	// Returns:
	//   True if read was successful, false otherwise.
	template<typename T>
	void ReadValue(T& value) {
		CheckFileOpen();

		stream >> value;
	}

	// Reads a piece of text from the currently open file
	// and skips to the next word afterwards.
	void ReadSkip(const std::string& text);

	std::string ReadSkipComponent();

	std::string ReadLine();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Checks if the file was opened correctly. If not, throws an exception.
	void CheckFileOpen();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	std::fstream stream;
	std::string filename;
	std::ios_base::openmode mode;
	unsigned indentLevel;
	const char* tab = "  ";
};
