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
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Subsystem.h>
#include <lfant/util/qumap.h>

// External
#include <bitset>

struct GLFWwindow;

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

//extern map<string, uint16_t> Key;

class Key_Initializer
{
public:
	Key_Initializer();
	uint16_t operator[](string in);
private:
	qumap<string, uint16_t> _key;
};

extern Key_Initializer Key;

/*
enum class Key : uint16_t
{
	Q = 'Q',
	W = 'W',
	E = 'E',
	R = 'R',
	T = 'T',
	Y = 'Y',
	U = 'U',
	I = 'I',
	O = 'O',
	P = 'P',
	A = 'A',
	S = 'S',
	D = 'D',
	F = 'F',
	G = 'G',
	H = 'H',
	J = 'J',
	K = 'K',
	L = 'L',
	Z = 'Z',
	X = 'X',
	C = 'C',
	V = 'V',
	B = 'B',
	N = 'N',
	M = 'M',
	F1 = GLFW_KEY_F1,
	F2 = GLFW_KEY_F2,
	F3 = GLFW_KEY_F3,
	F4 = GLFW_KEY_F4,
	F5 = GLFW_KEY_F5,
	F6 = GLFW_KEY_F6,
	F7 = GLFW_KEY_F7,
	F8 = GLFW_KEY_F8,
	F9 = GLFW_KEY_F9,
	F10 = GLFW_KEY_F10,
	F11 = GLFW_KEY_F11,
	F12 = GLFW_KEY_F12,
	Comma = ',',
	Dot = '.',
	Slash = '/',
	Exc = '!',
	Null = '\0',
	NewLine = '\n',
	Zero = '0',
	One = '1',
	Two = '2',
	Three = '3',
	Four = '4',
	Five = '5',
	Six = '6',
	Seven = '7',
	Eight = '8',
	Nine = '9',
	Space = GLFW_KEY_SPACE,
	Esc = GLFW_KEY_ESC,
	Tab = GLFW_KEY_TAB,
	Enter = GLFW_KEY_ENTER,
	Backspace = GLFW_KEY_BACKSPACE,
	Delete = GLFW_KEY_DEL,
	Insert = GLFW_KEY_INSERT,
	Home = GLFW_KEY_HOME,
	End = GLFW_KEY_END,
	PageUp = GLFW_KEY_PAGEUP,
	PageDown = GLFW_KEY_PAGEDOWN,
	Up = GLFW_KEY_UP,
	Down = GLFW_KEY_DOWN,
	Right = GLFW_KEY_RIGHT,
	Left = GLFW_KEY_LEFT,
	LShift = GLFW_KEY_LSHIFT,
	RShift = GLFW_KEY_RSHIFT,
	RCtrl = GLFW_KEY_RCTRL,
	LCtrl = GLFW_KEY_LCTRL,
	LAlt = GLFW_KEY_LALT,
	RAlt = GLFW_KEY_RALT,
	LSuper = GLFW_KEY_LSUPER,
	RSuper = GLFW_KEY_RSUPER,
	NumEnter = GLFW_KEY_KP_ENTER,
	MouseLeft = GLFW_MOUSE_BUTTON_LEFT,
	MouseRight = GLFW_MOUSE_BUTTON_RIGHT,
	MouseMiddle = GLFW_MOUSE_BUTTON_MIDDLE
};
*/

/**	This class controls the input system.
 *		Receives input from GLFW and manages it, allowing the
 *		user to bind certain keys to certain functions.
 *	@todo
 *		Handle gamepad/joystick input alongside keyboard/mouse.
 */
class Input : public Subsystem
{
	friend class Game;

	struct Axis
	{
		string name = "NewAxis";
		uint16_t positive;
		uint16_t negative;
		uint16_t positiveAlt;
		uint16_t negativeAlt;
		float sensitivity = 3.0f;
		float dead = 0.001f;
		bool snap = true;
		/// @todo Joystick axis

		/// Number of the controller to use. 0 means all.
		byte joyNum = 0;

		float value = 0.0f;
		bool posHeld = false;
		bool negHeld = false;
		bool down = false;
		bool up = false;

		Axis(string name) :
			name(name)
		{
		}

		Axis(string name, uint16_t positive, uint16_t negative = '\n', float sensitivity = 3.0f, float dead = 0.001f, bool snap =
				 true, byte joyNum = 0) :
			name(name), positive(positive), negative(negative), sensitivity(sensitivity), dead(dead), snap(snap), joyNum(
				joyNum)
		{
		}

		Axis(string name, uint16_t positive, uint16_t negative = '\n', uint16_t altpos = '\n', uint16_t altneg = '\n',
			 float sensitivity = 3.0f, float dead = 0.001f, bool snap = true, byte joyNum = 0) :
			name(name), positive(positive), negative(negative), positiveAlt(altpos), negativeAlt(altneg), sensitivity(
				sensitivity), dead(dead), snap(snap), joyNum(joyNum)
		{
		}

		~Axis()
		{
		}
	};

	struct Joystick
	{
		const float* values;
		int count;
	};

	struct Touch
	{
		vec2 pos;
	};

public:
	Input();
	~Input();

	virtual void Init();
	virtual void Update();

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop) const;

	void OnKeyPress(int key, int scancode, int action, int mods);
	void OnCharPress(uint32_t key);
	void OnMouseMove(vec2 pos);
	void OnMouseButton(int btn, int action, int mods);

	void AddAxis(string name, string positive = "null", string negative = "null", string altpos = "null", string altneg = "null", float sens = 3.0f, float dead = 0.001f, bool snap = true, byte joyNum = 0);

	// Axes
	float GetAxis(string name) const;

	// Buttons: Positive side of the given axis
	int8_t GetButton(string name) const;
	int8_t GetButtonDown(string name) const;
	int8_t GetButtonUp(string name) const;

#if !ANDROID
	ivec2 GetMousePos() const;
	void SetMousePos(ivec2 pos);
	void SetMousePos(int32 x, int32 y);
#else
	deque<Touch>& GetTouches();
	Touch& GetTouch(uint32_t idx);
	void OnTouch(uint32_t idx, int action, Touch data);
#endif

	void GetJoystickAxes();

	bool lockMouse;
	float mouseSpeed;

protected:
	deque<Axis> axes;
	deque<Joystick> joysticks;
#if ANDROID
	deque<Touch> touches;
#endif

	/// The string of input this frame.
	string inputString;
	ivec2 mousePos;

	//bitset<sizeof(byte)> keysHeld;

private:

	/** Called when any key is pressed or released.
	 *	@param key The key that was used.
	 *	@param mode The way the key was used. 0 = Release; 1 = Press;
	 */
	static void OnKeyPress(GLFWwindow* win, int key, int scancode, int action, int mods);
	static void OnCharPress(GLFWwindow* win, uint32_t key);
	static void OnMouseMove(GLFWwindow* win, double x, double y);
	static void OnMouseButton(GLFWwindow* win, int btn, int action, int mods);
};

/** @} */
/** @} */
}
