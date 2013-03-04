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
#include <GL/glfw.h>

// Internal

#include <lfant/Engine.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>

#include <lfant/Console.h>
#include <lfant/UserInterface.h>

namespace lfant
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
	Log("Input::Init: Begin.");
	glfwSetKeyCallback(OnKeyPress);
	glfwSetMousePosCallback(OnMouseMove);
	glfwSetMouseButtonCallback(OnMouseButton);
	glfwSetCharCallback(OnCharPress);

	Log("Input: Initialized");
}

void Input::Update()
{
	for(auto & axis : axes)
	{
		if(axis.down || axis.up)
		{
			Trigger(axis.name, axis.value);
			Trigger(axis.name, axis.name, axis.value);
			Trigger("All", axis.name, axis.value);
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

/*******************************************************************************
 *		Callback Functions
 *			KeyPress();
 *		\area Callback
 *******************************************************************************/

void GLFWCALL Input::OnKeyPress(int key, int mode)
{
	game->input->Trigger("KeyPress", (uint16_t)key, mode);
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

void GLFWCALL Input::OnMouseMove(int x, int y)
{
	game->input->Trigger("MouseMove", x, y);
	if(game->input->lockMouse)
	{
		game->input->SetMousePos(game->renderer->GetResolution() / 2);
	}
//	game->userInterface->OnMouseMove((float)x, (float)y);
}

void GLFWCALL Input::OnMouseButton(int btn, int mode)
{
	game->input->Trigger("MouseButton", btn, mode);
	OnKeyPress(btn, mode);
}

void GLFWCALL Input::OnCharPress(int key, int mode)
{
	game->input->Trigger("CharPress", key);
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
