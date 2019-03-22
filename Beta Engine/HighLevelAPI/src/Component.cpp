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

void Component::AddVarsToTweakBar(TwBar * bar)
{
	UNREFERENCED_PARAMETER(bar);

	/*std::string params = " label='" + GetName() + "' ";
	
	TwAddButton(bar, std::string("LabelButton_" + GetName()).c_str(), NULL, NULL, params.c_str());
	TwAddSeparator(bar, std::string("Separator_" + GetName()).c_str(), NULL);*/
}

GameObject * Component::GetOwner() const
{
	return static_cast<GameObject*>(GetParent());
}
