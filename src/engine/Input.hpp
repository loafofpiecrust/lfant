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
#pragma once
#include "stdafx.hpp"

// External
#include <GL/glfw.h>

// Internal
#include "Subsystem.hpp"
#include "BitField.hpp"

namespace sfs
{
	/** @addtogroup Engine
	 *	 @{
	 */
	/** @addtogroup Subsystems
	 *	 @{
	 */

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
		Dot = '.',
		Comma = ',',
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
		RSuper = GLFW_KEY_RSUPER
	};

	/// The type used for Handlers. A simple function object.
	typedef boost::function<void(int)> HandlerFunc;

	struct Binding
	{
		string action;
		vector<char> keys;
		vector<HandlerFunc> funcs;
		float value;
	};

	struct Handler
	{
		string action;
		HandlerFunc func;
		bool held = false;

		Handler(string action, HandlerFunc func) :
			action(action), func(func)
		{
		}
	};

	struct Axis
	{
		string name = "NewAxis";
		uint16_t positive;
		uint16_t negative;
		uint16_t altPositive;
		uint16_t altNegative;
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

		Axis(string name, uint16_t positive, uint16_t negative = '\n', float sensitivity = 3.0f, float dead = 0.001f, bool snap =
				 true, byte joyNum = 0) :
			name(name), positive(positive), negative(negative), sensitivity(sensitivity), dead(dead), snap(snap), joyNum(
				joyNum)
		{
		}

		Axis(string name, uint16_t positive, uint16_t negative = '\n', uint16_t altpos = '\n', uint16_t altneg = '\n',
			 float sensitivity = 3.0f, float dead = 0.001f, bool snap = true, byte joyNum = 0) :
			name(name), positive(positive), negative(negative), altPositive(altpos), altNegative(altneg), sensitivity(
				sensitivity), dead(dead), snap(snap), joyNum(joyNum)
		{
		}

		~Axis()
		{
		}
	};

	/**	This class controls the input system.
	 *	@details
	 *		Receives input from GLFW and manages it, allowing the
	 *		user to bind certain keys to certain functions.
	 *	@todo
	 *		Handle gamepad/joystick input alongside keyboard/mouse.
	 */
	class Input : public Subsystem
	{
		friend class Engine;
	public:
		Input();
		~Input();

		virtual void Init();
		virtual void Update();

		/** Called when any key is pressed or released.
		 *	@param key The key that was used.
		 *	@param mode The way the key was used. 0 = Release; 1 = Press;
		 */
		static void GLFWCALL KeyPress(int key, int mode);

		static void GLFWCALL MouseMove(int32_t x, int32_t y);

		void AddAxis(string name, Key positive, Key negative = Key::NewLine, Key altpos = Key::NewLine, Key altneg = Key::NewLine,
					 float sens = 3.0f, float dead = 0.001f, bool snap = true, byte joyNum = 0);

		// Axes
		float GetAxis(string name);

		// Buttons: Positive side of the given axis
		bool GetButton(string name, bool positive = true);
		bool GetButtonDown(string name, bool positive = true);
		bool GetButtonUp(string name, bool positive = true);

		ivec2 GetMousePos();
		void SetMousePos(ivec2 pos);
		void SetMousePos(int32_t x, int32_t y);

		vector<Axis> axes;

		bool lockMouse;

		/// The string of input this frame.
		string inputString;

		float mouseSpeed;

		BitField<byte, byte> keysHeld;

	protected:

	private:
	};

	/** @} */
	/** @} */
}
