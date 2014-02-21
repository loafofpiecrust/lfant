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

Key_Initializer Key;

uint16 Key_Initializer::operator[](string in)
{
	if(in.size() == 1)
	{
		to_upper(in);
		return in[0];
	}
	else
	{
		to_lower(in);
	//	Log("Key key, ", _key[in], ", Esc: ", GLFW_KEY_ESCAPE);
		return _key[in];
	}
}

Input::Input() :
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
		//	TriggerEvent(axis.name);
			TriggerEvent(axis.name, axis.value);
			TriggerEvent("All", axis.name, axis.value);

			if(axis.down)
			{
				TriggerEvent(axis.name+"_Down");
			}
			else if(axis.up)
			{
				TriggerEvent(axis.name+"_Up");
			}
		}
		if(axis.down)
		{
			axis.down = false;
		}
		if(axis.up)
		{
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

	Log("Loading input props...");
//	deque<Properties*> binds = prop->GetChildren("axis");
//	Properties* binds = prop->GetChild("axes");
	Axis axis("");
	for(auto& b : prop->children)
	{
		if(!b->IsType("axis")) continue;

		axis.name = b->name;
		if(axis.name == "") continue;

		Log("Adding axis '"+axis.name+"'");

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

/*******************************************************************************
*
*		Return functions
*
*******************************************************************************/


/*******************************************************************************
*
*		Mouse Functions
*
*******************************************************************************/


}
