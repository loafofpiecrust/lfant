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

map<string, uint16_t> Key;
static Key_Initializer _keyinits;

Key_Initializer::Key_Initializer()
{
	printf("About to add keys\n");

	Key[""] = '\0';
	Key["q"] = 'Q';

	Key["w"] = 'W';
	Key["e"] = 'E';
	Key["r"] = 'R';
	Key["t"] = 'T';
	Key["y"] = 'Y';
	Key["u"] = 'U';
	Key["i"] = 'I';
	Key["o"] = 'O';
	Key["p"] = 'P';
	Key["a"] = 'A';
	Key["s"] = 'S';
	Key["d"] = 'D';
	Key["f"] = 'F';
	Key["g"] = 'G';
	Key["h"] = 'H';
	Key["j"] = 'J';
	Key["k"] = 'K';
	Key["l"] = 'L';
	Key["z"] = 'Z';
	Key["x"] = 'X';
	Key["c"] = 'C';
	Key["v"] = 'V';
	Key["b"] = 'B';
	Key["n"] = 'N';
	Key["m"] = 'M';
	Key["f1"] = GLFW_KEY_F1;
	Key["f2"] = GLFW_KEY_F2;
	Key["f3"] = GLFW_KEY_F3;
	Key["f4"] = GLFW_KEY_F4;
	Key["f5"] = GLFW_KEY_F5;
	Key["f6"] = GLFW_KEY_F6;
	Key["f7"] = GLFW_KEY_F7;
	Key["f8"] = GLFW_KEY_F8;
	Key["f9"] = GLFW_KEY_F9;
	Key["f10"] = GLFW_KEY_F10;
	Key["f11"] = GLFW_KEY_F11;
	Key["f12"] = GLFW_KEY_F12;
	Key[","] = ',';
	Key["."] = '.';
	Key["/"] = '/';
	Key["!"] = '!';
	Key["null"] = '\0';
	Key["\n"] = '\n';
	Key["0"] = '0';
	Key["1"] = '1';
	Key["2"] = '2';
	Key["3"] = '3';
	Key["4"] = '4';
	Key["5"] = '5';
	Key["6"] = '6';
	Key["7"] = '7';
	Key["8"] = '8';
	Key["9"] = '9';
	Key["space"] = GLFW_KEY_SPACE;
	Key[" "] = GLFW_KEY_SPACE;
	Key["esc"] = GLFW_KEY_ESC;
	Key["tab"] = GLFW_KEY_TAB;
	Key["enter"] = GLFW_KEY_ENTER;
	Key["return"] = GLFW_KEY_ENTER;
	Key["backspace"] = GLFW_KEY_BACKSPACE;
	Key["back"] = GLFW_KEY_BACKSPACE;
	Key["delete"] = GLFW_KEY_DEL;
	Key["insert"] = GLFW_KEY_INSERT;
	Key["home"] = GLFW_KEY_HOME;
	Key["end"] = GLFW_KEY_END;
	Key["pageup"] = GLFW_KEY_PAGEUP;
	Key["pagedown"] = GLFW_KEY_PAGEDOWN;
	Key["up"] = GLFW_KEY_UP;
	Key["down"] = GLFW_KEY_DOWN;
	Key["right"] = GLFW_KEY_RIGHT;
	Key["left"] = GLFW_KEY_LEFT;
	Key["lshift"] = GLFW_KEY_LSHIFT;
	Key["rshift"] = GLFW_KEY_RSHIFT;
	Key["rctrl"] = GLFW_KEY_RCTRL;
	Key["lctrl"] = GLFW_KEY_LCTRL;
	Key["lalt"] = GLFW_KEY_LALT;
	Key["ralt"] = GLFW_KEY_RALT;
	Key["lsuper"] = GLFW_KEY_LSUPER;
	Key["rsuper"] = GLFW_KEY_RSUPER;
	Key["numenter"] = GLFW_KEY_KP_ENTER;
	Key["mouseleft"] = GLFW_MOUSE_BUTTON_LEFT;
	Key["mouseright"] = GLFW_MOUSE_BUTTON_RIGHT;
	Key["mousemiddle"] = GLFW_MOUSE_BUTTON_MIDDLE;

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

void Input::Load()
{
	Properties root("settings/input.cfg");
	Properties* prop = root.GetChild("input");

	vector<Properties*> binds = prop->GetChildren("axis");
	Axis* axis = nullptr;
	for(auto& i : binds)
	{
		printf("Loading an axis %s\n", i->id.c_str());
		axis = new Axis(i->id);

		axis->positive = Key[i->Get<string>("positive")];
		axis->negative = Key[i->Get<string>("negative")];
		axis->positiveAlt = Key[i->Get<string>("positiveAlt")];
		axis->positiveAlt = Key[i->Get<string>("negativeAlt")];

		axis->sensitivity = i->Get<float>("sensitivity", 3.0f);
		axis->dead = i->Get<float>("dead", 0.001f);
		axis->snap = i->Get<bool>("snap", true);
		axis->joyNum = i->Get<int>("joyNum", 0);
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
