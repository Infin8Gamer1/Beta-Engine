//------------------------------------------------------------------------------
//
// File Name:	Component.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(const std::string & name) : BetaObject(name), Serializable()
{
}

GameObject * Component::GetOwner() const
{
	return static_cast<GameObject*>(GetParent());
}
