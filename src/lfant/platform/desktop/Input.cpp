
#include <lfant/Input.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Renderer.h>
#include <lfant/Console.h>
#include "lfant/Window.h"

// External
#include <boost/algorithm/string.hpp>
#include <GLFW/glfw3.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace lfant {

void KeyMap::insert(string key, uint16 val)
{
	boost::to_lower(key);
	_key.insert(value_type::value_type(key, val));
}

KeyMap::KeyMap()
{
	insert("", -1);
	insert("f1", GLFW_KEY_F1);
	insert("f2", GLFW_KEY_F2);
	insert("f3", GLFW_KEY_F3);
	insert("f4", GLFW_KEY_F4);
	insert("f5", GLFW_KEY_F5);
	insert("f6", GLFW_KEY_F6);
	insert("f7", GLFW_KEY_F7);
	insert("f8", GLFW_KEY_F8);
	insert("f9", GLFW_KEY_F9);
	insert("f10", GLFW_KEY_F10);
	insert("f11", GLFW_KEY_F11);
	insert("f12", GLFW_KEY_F12);
	insert("esc", GLFW_KEY_ESCAPE);
	insert("tab", GLFW_KEY_TAB);
	insert("enter", GLFW_KEY_ENTER);
	insert("backspace", GLFW_KEY_BACKSPACE);
	insert("delete", GLFW_KEY_DELETE);
	insert("insert", GLFW_KEY_INSERT);
	insert("home", GLFW_KEY_HOME);
	insert("end", GLFW_KEY_END);
	insert("pageup", GLFW_KEY_PAGE_UP);
	insert("pagedown", GLFW_KEY_PAGE_DOWN);
	insert("up", GLFW_KEY_UP);
	insert("down", GLFW_KEY_DOWN);
	insert("right", GLFW_KEY_RIGHT);
	insert("left", GLFW_KEY_LEFT);
	insert("lshift", GLFW_KEY_LEFT_SHIFT);
	insert("rshift", GLFW_KEY_RIGHT_SHIFT);
	insert("rctrl", GLFW_KEY_RIGHT_CONTROL);
	insert("lctrl", GLFW_KEY_LEFT_CONTROL);
	insert("lalt", GLFW_KEY_LEFT_ALT);
	insert("ralt", GLFW_KEY_RIGHT_ALT);
	insert("lsuper", GLFW_KEY_LEFT_SUPER);
	insert("rsuper", GLFW_KEY_RIGHT_SUPER);
	insert("numenter", GLFW_KEY_KP_ENTER);
	insert("mouseleft", 400+GLFW_MOUSE_BUTTON_LEFT);
	insert("mouseright", 400+GLFW_MOUSE_BUTTON_RIGHT);
	insert("mousemiddle", 400+GLFW_MOUSE_BUTTON_MIDDLE);

	string str = " ";
	for(char c = 'a'; c <= 'z'; ++c)
	{
		str[0] = c;
	//	insert(str, sf::Keyboard::A + (c - 'a'));
		insert(str, GLFW_KEY_A + (c - 'a'));
	}

	for(char c = '0'; c <= '9'; ++c)
	{
		str[0] = c;
	//	insert(str, sf::Keyboard::Num0 + (c - '0'));
		insert(str, GLFW_KEY_0 + (c - '0'));
	}

/*	insert("[", sf::Keyboard::LBracket);
	insert("]", sf::Keyboard::RBracket);
	insert("-", sf::Keyboard::Dash);
	insert("=", sf::Keyboard::Equal);
	insert("`", sf::Keyboard::Tilde);
	insert(",", sf::Keyboard::Comma);
	insert(".", sf::Keyboard::Period);
	insert("/", sf::Keyboard::Slash);
	insert("\\", sf::Keyboard::BackSlash);
	insert(";", sf::Keyboard::SemiColon);
	insert("\'", sf::Keyboard::Quote);

	insert("f1", sf::Keyboard::F1);
	insert("f2", sf::Keyboard::F2);
	insert("f3", sf::Keyboard::F3);
	insert("f4", sf::Keyboard::F4);
	insert("f5", sf::Keyboard::F5);
	insert("f6", sf::Keyboard::F6);
	insert("f7", sf::Keyboard::F7);
	insert("f8", sf::Keyboard::F8);
	insert("f9", sf::Keyboard::F9);
	insert("f10", sf::Keyboard::F10);
	insert("f11", sf::Keyboard::F11);
	insert("f12", sf::Keyboard::F12);
	insert("esc", sf::Keyboard::Escape);
	insert("tab", sf::Keyboard::Tab);
	insert("enter", sf::Keyboard::Return);
	insert("backspace", sf::Keyboard::BackSpace);
	insert("delete", sf::Keyboard::Delete);
	insert("insert", sf::Keyboard::Insert);
	insert("home", sf::Keyboard::Home);
	insert("end", sf::Keyboard::End);
	insert("pageup", sf::Keyboard::PageUp);
	insert("pagedown", sf::Keyboard::PageDown);
	insert("up", sf::Keyboard::Up);
	insert("down", sf::Keyboard::Down);
	insert("right", sf::Keyboard::Right);
	insert("left", sf::Keyboard::Left);
	insert("lshift", sf::Keyboard::LShift);
	insert("rshift", sf::Keyboard::RShift);
	insert("rctrl", sf::Keyboard::RControl);
	insert("lctrl", sf::Keyboard::LControl);
	insert("lalt", sf::Keyboard::LAlt);
	insert("ralt", sf::Keyboard::RAlt);
	insert("lsuper", sf::Keyboard::LSystem);
	insert("rsuper", sf::Keyboard::RSystem);
	insert("mouseleft", 400+sf::Mouse::Left);
	insert("mouseright", 400+sf::Mouse::Right);
	insert("mousemiddle", 400+sf::Mouse::Middle);*/


	/// @todo Needed?
	insert("joy1", 500);
	insert("joy2", 501);
	insert("joy3", 502);
	insert("joy4", 503);
	insert("joy5", 504);
}

void Input::Init()
{
	Subsystem::Init();
	LoadFile("settings/input.prop");
}

void Input::OnKeyPress(int key, int action, int mods)
{
//	GetGame()->Log("Key pressed, ", (char)key);
	TriggerEvent("KeyPress", (uint16)key, action);
	TriggerEvent("KeyPress", (uint16)key, action, mods);
	for(auto& axis : axes)
	{
		if(key == axis.positive)
		{
			axis.posHeld = action;
			if(action == 1)
			{
				axis.down = true;
				if(axis.snap)
				{
					//axis.value = axis.negHeld ? 0.0f : 1.0f;
					axis.value += 1.0f;
				}
				TriggerEvent(axis.name+"_Down");
			}
			else if(action == 0)
			{
				axis.up = true;
				if(axis.snap)
				{
					//axis.value = axis.negHeld ? -1.0f : 0.0f;
					axis.value -= 1.0f;
				}
				TriggerEvent(axis.name+"_Up");
			}
			TriggerEvent(axis.name, axis.value);
		}
		else if(key == axis.negative)
		{
			axis.negHeld = action;
			if(action == 1)
			{
				if(axis.snap)
				{
					//axis.value = axis.posHeld ? 0.0f : -1.0f;
					axis.value -= 1.0f;
				}
			}
			else if(action == 0)
			{
				if(axis.snap)
				{
					//axis.value = axis.posHeld ? 1.0f : 0.0f;
					axis.value += 1.0f;
				}
			}
			TriggerEvent(axis.name, axis.value);
		}
	}
}

void Input::OnMouseMove(vec2 pos)
{
	TriggerEvent("MouseMove", (int)pos.x, (int)pos.y);
	TriggerEvent("MouseMove", (ivec2)pos);
	TriggerEvent("MouseMove", pos);
	if(lockMouse)
	{
		SetMousePos(game->window->GetSize() / 2);
	}
//	game->userInterface->OnMouseMove((float)x, (float)y);
}

void Input::OnCursorEnter(bool entered)
{
	TriggerEvent("CursorEnter", entered);
}

void Input::OnScroll(vec2 offset)
{
	TriggerEvent("Scroll", offset);
}

void Input::OnCharPress(uint32 key)
{
	TriggerEvent("CharPress", (char)key);
}

void Input::OnMouseButton(int btn, int action, int mods)
{
	std::cout << "Mouse button at " << lexical_cast<string>(GetMousePos()) << "\n";

	TriggerEvent("MouseButton", (uint16)btn, action);
//	TriggerEvent("MouseButton", (uint16)btn, action, mods);
	OnKeyPress(400+btn, action, mods);
}

void Input::GetJoystickAxes()
{
//	float* values = nullptr;
	int axisCount = 0;
	int joyCount = 0;
	for(uint i = 0; i < 16; ++i)
	{
	//	if(glfwJoystickPresent(i))
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
	//	joysticks[i].values = glfwGetJoystickAxes(i, &axisCount);
		joysticks[i].count = axisCount;
		for(uint k = 0; k < axisCount; ++k)
		{
			GetGame()->Log("Joystick value ", k, " = ", joysticks[i].values[k]);
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

int8 Input::GetButton(string name) const
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

int8 Input::GetButtonDown(string name) const
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
	else
	{
		return 0;
	}
}

int8 Input::GetButtonUp(string name) const
{
	Axis* axis = GetAxis(name);
	if(!axis) return 0;

	return axis->up;
}

int8 Input::GetKeyDown(uint16 key) const
{
	return 0;
}

ivec2 Input::GetMousePos() const
{
	if(game->standAlone)
	{
		return game->window->GetCursorPos();
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
	//	glfwSetCursorPos(game->renderer->GetWindowHandle(), x, y);
		game->window->SetCursorPos({x, y});
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
