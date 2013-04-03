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

#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>
#include <lfant/Properties.h>
#include <lfant/Console.h>
#include <lfant/UserInterface.h>

namespace lfant
{

Key_Initializer Key;

uint16 Key_Initializer::operator [](string in)
{
	to_lower(in);
	return _key[in];
}

Key_Initializer::Key_Initializer()
{

	_key[""] = '\0';
	_key["q"] = 'Q';

	_key["w"] = 'W';
	_key["e"] = 'E';
	_key["r"] = 'R';
	_key["t"] = 'T';
	_key["y"] = 'Y';
	_key["u"] = 'U';
	_key["i"] = 'I';
	_key["o"] = 'O';
	_key["p"] = 'P';
	_key["a"] = 'A';
	_key["s"] = 'S';
	_key["d"] = 'D';
	_key["f"] = 'F';
	_key["g"] = 'G';
	_key["h"] = 'H';
	_key["j"] = 'J';
	_key["k"] = 'K';
	_key["l"] = 'L';
	_key["z"] = 'Z';
	_key["x"] = 'X';
	_key["c"] = 'C';
	_key["v"] = 'V';
	_key["b"] = 'B';
	_key["n"] = 'N';
	_key["m"] = 'M';
	_key["f1"] = GLFW_KEY_F1;
	_key["f2"] = GLFW_KEY_F2;
	_key["f3"] = GLFW_KEY_F3;
	_key["f4"] = GLFW_KEY_F4;
	_key["f5"] = GLFW_KEY_F5;
	_key["f6"] = GLFW_KEY_F6;
	_key["f7"] = GLFW_KEY_F7;
	_key["f8"] = GLFW_KEY_F8;
	_key["f9"] = GLFW_KEY_F9;
	_key["f10"] = GLFW_KEY_F10;
	_key["f11"] = GLFW_KEY_F11;
	_key["f12"] = GLFW_KEY_F12;
	_key[","] = ',';
	_key["."] = '.';
	_key["/"] = '/';
	_key["!"] = '!';
	_key["null"] = '\0';
	_key["\n"] = '\n';
	_key["0"] = '0';
	_key["1"] = '1';
	_key["2"] = '2';
	_key["3"] = '3';
	_key["4"] = '4';
	_key["5"] = '5';
	_key["6"] = '6';
	_key["7"] = '7';
	_key["8"] = '8';
	_key["9"] = '9';
	_key["space"] = GLFW_KEY_SPACE;
	_key[" "] = GLFW_KEY_SPACE;
	_key["esc"] = GLFW_KEY_ESC;
	_key["tab"] = GLFW_KEY_TAB;
	_key["enter"] = GLFW_KEY_ENTER;
	_key["return"] = GLFW_KEY_ENTER;
	_key["backspace"] = GLFW_KEY_BACKSPACE;
	_key["back"] = GLFW_KEY_BACKSPACE;
	_key["delete"] = GLFW_KEY_DEL;
	_key["insert"] = GLFW_KEY_INSERT;
	_key["home"] = GLFW_KEY_HOME;
	_key["end"] = GLFW_KEY_END;
	_key["pageup"] = GLFW_KEY_PAGEUP;
	_key["pagedown"] = GLFW_KEY_PAGEDOWN;
	_key["up"] = GLFW_KEY_UP;
	_key["down"] = GLFW_KEY_DOWN;
	_key["right"] = GLFW_KEY_RIGHT;
	_key["left"] = GLFW_KEY_LEFT;
	_key["lshift"] = GLFW_KEY_LSHIFT;
	_key["rshift"] = GLFW_KEY_RSHIFT;
	_key["rctrl"] = GLFW_KEY_RCTRL;
	_key["lctrl"] = GLFW_KEY_LCTRL;
	_key["lalt"] = GLFW_KEY_LALT;
	_key["ralt"] = GLFW_KEY_RALT;
	_key["lsuper"] = GLFW_KEY_LSUPER;
	_key["rsuper"] = GLFW_KEY_RSUPER;
	_key["numenter"] = GLFW_KEY_KP_ENTER;
	_key["mouseleft"] = GLFW_MOUSE_BUTTON_LEFT;
	_key["mouseright"] = GLFW_MOUSE_BUTTON_RIGHT;
	_key["mousemiddle"] = GLFW_MOUSE_BUTTON_MIDDLE;

}

Input::Input() :
	lockMouse(false), mouseSpeed(0.005f)
{
}

Input::~Input()
{
}

void Input::Init()
{
	Subsystem::Init();

	Log("Input::Init: Begin.");
	glfwSetKeyCallback(OnKeyPress);
	glfwSetMousePosCallback(OnMouseMove);
	glfwSetMouseButtonCallback(OnMouseButton);
	glfwSetCharCallback(OnCharPress);

	Log("Input: Initialized");
}

void Input::Update()
{
	for(auto& axis : axes)
	{
		if(axis.down || axis.up)
		{
			TriggerEvent(axis.name, axis.value);
			TriggerEvent(axis.name, axis.name, axis.value);
			TriggerEvent("All", axis.name, axis.value);
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
//	Properties root("settings/input.cfg");
//	Properties* prop = root.GetChild("input");
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

/*******************************************************************************
*		Callback Functions
*			KeyPress();
*		\area Callback
*******************************************************************************/

void GLFWCALL Input::OnKeyPress(int key, int mode)
{
	game->input->TriggerEvent("KeyPress", (uint16_t)key, mode);
	for(auto& axis : game->input->axes)
	{
		if(key == axis.positive || key == axis.positiveAlt)
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
		else if(key == axis.negative || key == axis.negativeAlt)
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
	game->input->TriggerEvent("MouseMove", x, y);
	if(game->input->lockMouse)
	{
		game->input->SetMousePos(game->renderer->GetResolution() / 2);
	}
//	game->userInterface->OnMouseMove((float)x, (float)y);
}

void GLFWCALL Input::OnMouseButton(int btn, int mode)
{
	game->input->TriggerEvent("MouseButton", btn, mode);
	OnKeyPress(btn, mode);
}

void GLFWCALL Input::OnCharPress(int key, int mode)
{
	game->input->TriggerEvent("CharPress", key);
}

/*******************************************************************************
*
*		Return functions
*
*******************************************************************************/

void Input::AddAxis(string name, uint16_t positive, uint16_t negative, uint16_t altpos, uint16_t altneg, float sens, float dead, bool snap,
					byte joyNum)
{
	axes.push_back(Axis(name, positive, negative, altpos, altneg, sens, dead, snap, joyNum));
}

float Input::GetAxis(string name)
{
	for(auto& axis : axes)
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
	for(auto& axis : axes)
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
