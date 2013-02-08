/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include "Input.hpp"

// External
#include <iostream>

// Internal
#include "Engine.hpp"
#include "Time.hpp"
#include "Renderer.hpp"
#include "Console.hpp"

namespace sfs
{

Input::Input() :
	lockMouse(false), mouseSpeed(0.005f)
{
}

Input::~Input()
{
}

void Input::Init()
{
	glfwSetKeyCallback(KeyPress);
	glfwSetMousePosCallback(MouseMove);

	Log("Input: Initialized");
}

void Input::Update()
{
	for(auto & axis : axes)
	{
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

/*******************************************************************************
 *		Callback Functions
 *			KeyPress();
 *		@area Callback
 *******************************************************************************/

void GLFWCALL Input::KeyPress(int key, int mode)
{
	for(auto & axis : game->input->axes)
	{
		if(key == axis.positive || key == axis.altPositive)
		{
			if(mode == 1)
			{
				axis.posHeld = true;
				axis.down = true;
				if(axis.snap)
				{
					if(axis.negHeld)
					{
						axis.value = 0.0f;
					}
					else
					{
						axis.value = 1.0f;
					}
				}
			}
			else
			{
				axis.posHeld = false;
				axis.up = true;
				if(axis.snap)
				{
					if(axis.negHeld)
					{
						axis.value = -1.0f;
					}
					else
					{
						axis.value = 0.0f;
					}
				}
			}
		}
		else if(key == axis.negative || key == axis.altNegative)
		{
			if(mode == 1)
			{
				axis.negHeld = true;
				if(axis.snap)
				{
					if(axis.posHeld)
					{
						axis.value = 0.0f;
					}
					else
					{
						axis.value = -1.0f;
					}
				}
			}
			else
			{
				axis.negHeld = false;
				if(axis.snap)
				{
					if(axis.posHeld)
					{
						axis.value = 1.0f;
					}
					else
					{
						axis.value = 0.0f;
					}
				}
			}
		}
	}
}

void GLFWCALL Input::MouseMove(int32_t x, int32_t y)
{
	if(game->input->lockMouse)
	{
		game->input->SetMousePos(game->renderer->resolution / 2);
	}
}

/*******************************************************************************
 *
 *		Return functions
 *
 *******************************************************************************/

void Input::AddAxis(string name, Key positive, Key negative, Key altpos, Key altneg, float sens, float dead, bool snap,
					byte joyNum)
{
	axes.push_back(
		Axis(name, (uint16_t)positive, (uint16_t)negative, (uint16_t)altpos, (uint16_t)altneg, sens, dead, snap, joyNum));
}

float Input::GetAxis(string name)
{
	for(auto & axis : axes)
	{
		if(axis.name == name)
		{
			/*if (abs(axis.value) <= axis.dead)
			 {
			 return 0.0f;
			 }*/
			return axis.value;
		}
	}
	return 0.0f;
}

bool Input::GetButton(string name, bool positive)
{
	for(auto axis : axes)
	{
		if(axis.name == name)
		{
			if(positive)
			{
				if(axis.value > axis.dead && axis.posHeld)
				{
					return true;
				}
			}
			else
			{
				if(axis.value < -axis.dead && axis.negHeld)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Input::GetButtonDown(string name, bool positive)
{
	for(auto & axis : axes)
	{
		if(axis.name == name)
		{
			if(positive)
			{
				if(axis.posHeld)
				{
					return axis.down;
				}
			}
			else
			{
				if(axis.negHeld)
				{
					return axis.down;
				}
			}
		}
	}
	return false;
}

bool Input::GetButtonUp(string name, bool positive)
{
	for(auto axis : axes)
	{
		if(axis.name == name)
		{
			return axis.up;
		}
	}
	return false;
}

/*******************************************************************************
 *
 *		Mouse Functions
 *
 *******************************************************************************/

ivec2 Input::GetMousePos()
{
	ivec2 result;
	glfwGetMousePos(&result.x, &result.y);
	return result;
}

void Input::SetMousePos(int32_t x, int32_t y)
{
	glfwSetMousePos(x, y);
}

void Input::SetMousePos(ivec2 pos)
{
	glfwSetMousePos(pos.x, pos.y);
}

}
