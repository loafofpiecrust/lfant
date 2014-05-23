/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-16 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Input.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>
#include <lfant/Properties.h>
#include <lfant/Console.h>
#include <lfant/UserInterface.h>

// External
#include <iostream>


using namespace boost::algorithm;

namespace lfant
{

KeyMap Key;

uint16 KeyMap::operator[](string in)
{
/*	if(in.size() == 1)
	{
		to_upper(in);
		return in[0];
	}
	auto dat = _key.left.find(in);
	if(dat != _key.left.end())
	{
		return dat->second;
	}
	else
	{
		return 0;
	}
	*/
	to_lower(in);
	return _key[in];
}

string KeyMap::operator[](uint16 c)
{
/*	auto dat = _key.right.find(c);
	if(dat != _key.right.end())
	{
		return dat->second;
	}
	else
	{
		return "";
	}
	*/
	return "";
}

Input::Input(Game* game) :
	Subsystem(game),
	lockMouse(false),
	mouseSpeed(0.005f)
{
}

Input::~Input()
{
}

void Input::Update()
{
	for(auto& axis : axes)
	{
		if(axis.down || axis.up)
		{
			axis.down = false;
			axis.up = false;
		}
		if(axis.snap || (abs(axis.value) <= axis.dead && !axis.posHeld && !axis.negHeld))
		{
			continue;
		}

		if(axis.posHeld && !axis.negHeld)
		{
			axis.value += axis.sensitivity * game->time->deltaTime;
		}
		else if(!axis.posHeld && axis.negHeld)
		{
			axis.value += axis.sensitivity * game->time->deltaTime;
		}
		else
		{
			if(axis.value > axis.dead)
			{
				axis.value -= axis.sensitivity * game->time->deltaTime * 1.75;
			}
			else if(axis.value < -axis.dead)
			{
				axis.value += axis.sensitivity * game->time->deltaTime * 1.75;
			}
		}

		if(abs(axis.value) > 1.0f)
		{
			axis.value = clamp(axis.value, -1.0f, 1.0f);
		}
	}
}

void Input::Load(Properties* prop)
{
	Subsystem::Load(prop);

	GetGame()->Log("Loading input props...");
//	deque<Properties*> binds = prop->GetChildren("axis");
//	Properties* binds = prop->GetChild("axes");
	for(auto& b : prop->children)
	{
		if(!b->IsType("axis") || b->name.empty()) continue;

		Axis axis {b->name};

		GetGame()->Log("Adding axis '"+axis.name+"' mapped to ", (char)Key[b->Get("positive")]);

		axis.positive = Key[b->Get("positive")];
		axis.negative = Key[b->Get("negative")];
	//	axis.positiveAlt = Key[i->Get<string>("positiveAlt")];
	//	axis.positiveAlt = Key[i->Get<string>("negativeAlt")];

	//	axis.sensitivity = .asFloat();
		b->Get("sensitivity", axis.sensitivity);
		b->Get("dead", axis.dead);
		b->Get("snap", axis.snap);
		b->Get("joyNum", axis.joyNum);

		axes.push_back(axis);
	}
}

void Input::Save(Properties* prop) const
{
	Subsystem::Save(prop);
}



}
