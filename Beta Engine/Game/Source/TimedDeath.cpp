//------------------------------------------------------------------------------
//
// File Name:	TimedDeath.cpp
// Author(s):	Jacob Holyfield
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "TimedDeath.h"
#include "GameObject.h"
#include <Parser.h>

Behaviors::TimedDeath::TimedDeath(float _timeUntilDeath) : Component("TimedDeath")
{
	timeUntilDeath = _timeUntilDeath;
}

Component * Behaviors::TimedDeath::Clone() const
{
	return new TimedDeath(timeUntilDeath);
}

void Behaviors::TimedDeath::Deserialize(Parser & parser)
{
	parser.ReadVariable("timeUntilDeath", timeUntilDeath);
}

void Behaviors::TimedDeath::Serialize(Parser & parser) const
{
	parser.WriteVariable("timeUntilDeath", timeUntilDeath);
}

void Behaviors::TimedDeath::Update(float dt)
{
	timeUntilDeath -= dt;

	if (timeUntilDeath <= 0) {
		GetOwner()->Destroy();
	}
}
