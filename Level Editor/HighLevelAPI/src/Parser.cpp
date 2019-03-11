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

Parser::Parser(const std::string & _filename, std::ios_base::openmode mode) : stream(_filename, mode)
{	
	filename = _filename;
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

void Parser::CheckFileOpen()
{
	if (!stream.is_open()) {
		throw ParseException(filename, "Filestream isn't open! ERROR 3");
	}
}
