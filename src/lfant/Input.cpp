/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-16 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/

#include <lfant/Input.h>

// External
#include <iostream>
//#include <GLFW/glfw3.h>

// Internal

#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>
#include <lfant/Properties.h>
#include <lfant/Console.h>
#include <lfant/UserInterface.h>

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
	deque<Properties*> binds = prop->GetChildren("axis");
	Axis axis("");
	for(auto& i : binds)
	{
		axis.name = i->id;
		if(axis.name == "") continue;

		axis.positive = Key[i->Get<string>("positive")];
		axis.negative = Key[i->Get<string>("negative")];
		axis.positiveAlt = Key[i->Get<string>("positiveAlt")];
		axis.positiveAlt = Key[i->Get<string>("negativeAlt")];

		axes.push_back(axis);

		i->Get("sensitivity", axis.sensitivity);
		i->Get("dead", axis.dead);
		i->Get("snap", axis.snap);
		i->Get("joyNum", axis.joyNum);
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
