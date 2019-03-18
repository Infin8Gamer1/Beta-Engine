//------------------------------------------------------------------------------
//
// File Name:	Serializable.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Serializable.h"

void Serializable::Deserialize(Parser & parser)
{
	UNREFERENCED_PARAMETER(parser);
}

void Serializable::Serialize(Parser & parser) const
{
	UNREFERENCED_PARAMETER(parser);
}

Serializable::Serializable()
{
}

Serializable::~Serializable()
{
}

Serializable::Serializable(const Serializable & other)
{
	UNREFERENCED_PARAMETER(other);
}

Serializable & Serializable::operator=(const Serializable & other)
{
	UNREFERENCED_PARAMETER(other);
	return *this;
}
