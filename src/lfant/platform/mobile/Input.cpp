

#include <lfant/Input.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Renderer.h>

// External

namespace lfant {

Key_Initializer::Key_Initializer()
{
}

void Input::Init()
{
	Subsystem::Init();
}

void Input::OnKeyPress(GLFWwindow* win, int key, int scancode, int action, int mods)
{
}

void Input::OnCharPress(GLFWwindow* win, uint32_t key)
{
	game->input->TriggerEvent("CharPress", (char)key);
}

void Input::GetJoystickAxes()
{
	// No mobile joysticks
}



void Input::AddAxis(string name, string positive, string negative, string altpos, string altneg, float sens, float dead, bool snap, byte joyNum)
{
	// No axes on mobile
}

float Input::GetAxis(string name) const
{
	return 0.0f;
}

int8_t Input::GetButton(string name) const
{
	return 0;
}

int8_t Input::GetButtonDown(string name) const
{
	return 0;
}

int8_t Input::GetButtonUp(string name) const
{
	return 0;
}

deque<Input::Touch>& Input::GetTouches()
{
	return touches;
}

Input::Touch& Input::GetTouch(uint32_t idx)
{
	return touches[idx];
}

void Input::OnTouch(uint32_t idx, int action, Input::Touch data)
{
	switch(action)
	{
	case 0: // Down
	{
		touches[idx] = data;
		TriggerEvent("TouchDown", idx, data);
		break;
	}
	case 1: // Up
	{
		TriggerEvent("TouchUp", idx, data);
		touches.erase(touches.begin()+idx);
		break;
	}
	case 2: // Movement
	{
		// Too often of a trigger?
	//	TriggerEvent("TouchMove", idx, data);
		touches[idx] = data;
		break;
	}
	case 3: // Cancel (Up but don't count)
	{

	}
	default: // The others?
	{

	}
	}
}

}