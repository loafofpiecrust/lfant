
#include <lfant/Input.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Renderer.h>
#include <lfant/Console.h>

// External
#include <GLFW/glfw3.h>

namespace lfant {

Key_Initializer::Key_Initializer()
{
	_key[""] = '\0';
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
	/*
	_key[","] = ',';
	_key["."] = '.';
	_key["/"] = '/';
	*/
	_key["!"] = '!';
	_key["null"] = '\0';
	_key["\n"] = '\n';
	/*
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
	*/
	_key["space"] = GLFW_KEY_SPACE;
//	_key[" "] = GLFW_KEY_SPACE;
	_key["esc"] = GLFW_KEY_ESCAPE;
	_key["tab"] = GLFW_KEY_TAB;
	_key["enter"] = GLFW_KEY_ENTER;
	_key["return"] = GLFW_KEY_ENTER;
	_key["backspace"] = GLFW_KEY_BACKSPACE;
	_key["back"] = GLFW_KEY_BACKSPACE;
	_key["delete"] = GLFW_KEY_DELETE;
	_key["del"] = GLFW_KEY_DELETE;
	_key["insert"] = GLFW_KEY_INSERT;
	_key["home"] = GLFW_KEY_HOME;
	_key["end"] = GLFW_KEY_END;
	_key["pageup"] = GLFW_KEY_PAGE_UP;
	_key["pagedown"] = GLFW_KEY_PAGE_DOWN;
	_key["up"] = GLFW_KEY_UP;
	_key["down"] = GLFW_KEY_DOWN;
	_key["right"] = GLFW_KEY_RIGHT;
	_key["left"] = GLFW_KEY_LEFT;
	_key["lshift"] = GLFW_KEY_LEFT_SHIFT;
	_key["rshift"] = GLFW_KEY_RIGHT_SHIFT;
	_key["rctrl"] = GLFW_KEY_RIGHT_CONTROL;
	_key["lctrl"] = GLFW_KEY_LEFT_CONTROL;
	_key["lalt"] = GLFW_KEY_LEFT_ALT;
	_key["ralt"] = GLFW_KEY_RIGHT_ALT;
	_key["lsuper"] = GLFW_KEY_LEFT_SUPER;
	_key["rsuper"] = GLFW_KEY_RIGHT_SUPER;
	_key["numenter"] = GLFW_KEY_KP_ENTER;
	_key["mouseleft"] = 400+GLFW_MOUSE_BUTTON_LEFT;
	_key["mouseright"] = 400+GLFW_MOUSE_BUTTON_RIGHT;
	_key["mousemiddle"] = 400+GLFW_MOUSE_BUTTON_MIDDLE;

	_key["joy1"] = 500;
	_key["joy2"] = 501;
	_key["joy3"] = 502;
	_key["joy4"] = 503;
	_key["joy5"] = 504;
	_key["joy6"] = 505;
	_key["joy7"] = 506;
}

void Input::Init()
{
	Subsystem::Init();

	Log("Input::Init: Begin.");
	if(game->standAlone)
	{
		glfwSetKeyCallback(game->renderer->GetWindowHandle(), OnKeyPress);
		glfwSetCursorPosCallback(game->renderer->GetWindowHandle(), OnMouseMove);
		glfwSetMouseButtonCallback(game->renderer->GetWindowHandle(), OnMouseButton);
		glfwSetCharCallback(game->renderer->GetWindowHandle(), OnCharPress);
	}

	Log("Input: Initialized");
}

void Input::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	game->input->OnKeyPress(key, scancode, action, mods);
}

void Input::OnKeyPress(int key, int scancode, int action, int mods)
{
//	Log("Key pressed");
	TriggerEvent("KeyPress", (uint16_t)key, action);
	for(auto& axis : axes)
	{
		if(key == axis.positive)
		{
			if(action == GLFW_PRESS)
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
			else if(action == GLFW_RELEASE)
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
		else if(key == axis.negative)
		{
			if(action == GLFW_PRESS)
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
			else if(action == GLFW_RELEASE)
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

void Input::OnMouseMove(GLFWwindow* window, double x, double y)
{
	game->input->OnMouseMove(vec2(x, y));
}

void Input::OnMouseMove(vec2 pos)
{
	TriggerEvent("MouseMove", (int)pos.x, (int)pos.y);
	TriggerEvent("MouseMove", (ivec2)pos);
	TriggerEvent("MouseMove", pos);
	if(lockMouse)
	{
		SetMousePos(game->renderer->GetResolution() / 2);
	}
//	game->userInterface->OnMouseMove((float)x, (float)y);
}

void Input::OnCharPress(uint32_t key)
{
	TriggerEvent("CharPress", (char)key);
}

void Input::OnCharPress(GLFWwindow* win, uint32_t key)
{
	game->input->OnCharPress(key);
}

void Input::OnMouseButton(GLFWwindow* window, int btn, int action, int mods)
{
	game->input->OnMouseButton(btn, action, mods);
}

void Input::OnMouseButton(int btn, int action, int mods)
{
	Log("Mouse button at ", lexical_cast<string>(GetMousePos()));

	btn = 400+btn;
	TriggerEvent("MouseButton", (uint16_t)btn, action);
	if(action == GLFW_PRESS)
	{
		TriggerEvent("MouseDown", (uint16_t)btn);
	}
	else if(action == GLFW_RELEASE)
	{
		TriggerEvent("MouseUp", (uint16_t)btn);
	}
	OnKeyPress(btn, 0, action, mods);
}

void Input::GetJoystickAxes()
{
//	float* values = nullptr;
	int axisCount = 0;
	int joyCount = 0;
	for(uint i = 0; i < 16; ++i)
	{
		if(glfwJoystickPresent(i))
		{
			++joyCount;
		}
	}

	if(joysticks.size() != joyCount)
	{
		joysticks.resize(joyCount);
	}
	for(uint i = 0; i < joyCount; ++i)
	{
		joysticks[i].values = glfwGetJoystickAxes(i, &axisCount);
		joysticks[i].count = axisCount;
		for(uint k = 0; k < axisCount; ++k)
		{
			Log("Joystick value ", k, " = ", joysticks[i].values[k]);
		}
	}
}


void Input::AddAxis(Axis axis)
{
	axes.push_back(axis);
}

Input::Axis* Input::GetAxis(string name) const
{
	for(auto& axis : axes)
	{
		if(axis.name == name)
		{
			return const_cast<Input::Axis*>(&axis);
		}
	}
	return nullptr;
}

int8_t Input::GetButton(string name) const
{
	Axis* axis = GetAxis(name);
	if(!axis) return 0;

	if(axis->value > axis->dead && axis->posHeld)
	{
		return 1;
	}
	else if(axis->value < -axis->dead && axis->negHeld)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int8_t Input::GetButtonDown(string name) const
{
	Axis* axis = GetAxis(name);

	if(!axis->down) return false;

	if(axis->posHeld)
	{
		return axis->down;
	}
	else if(axis->negHeld)
	{
		return -axis->down;
	}
}

int8_t Input::GetButtonUp(string name) const
{
	Axis* axis = GetAxis(name);
	if(!axis) return 0;

	return axis->up;
}

ivec2 Input::GetMousePos() const
{
	if(game->standAlone)
	{
		dvec2 result;
		glfwGetCursorPos(game->renderer->GetWindowHandle(), &result.x, &result.y);
		return (ivec2)result;
	}
	else
	{
		return mousePos;
	}
}

void Input::SetMousePos(int32 x, int32 y)
{
	if(game->standAlone)
	{
		glfwSetCursorPos(game->renderer->GetWindowHandle(), x, y);
	}
	else
	{
		mousePos = ivec2(x, y);
	}
}

void Input::SetMousePos(ivec2 pos)
{
	SetMousePos(pos.x, pos.y);
}

}
