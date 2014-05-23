/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Subsystem.h>
#include <lfant/util/qumap.h>

// External
//#include <boost/bimap.hpp>
#include <unordered_map>
#include <bitset>

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

//extern map<string, uint16> Key;

class KeyMap
{
public:
	KeyMap();
	uint16 operator[](string in);
	string operator[](uint16 c);
private:
	void insert(string key, uint16 val);

//	typedef boost::bimap<string, uint16> value_type;
	typedef std::unordered_map<string, uint16> value_type;
	value_type _key;
//	std::unordered_map<string, uint16> _key;
};

extern KeyMap Key;

/**	This class controls the input system.
 *		Receives input from GLFW and manages it, allowing the
 *		user to bind certain keys to certain functions.
 *	@todo
 *		Handle gamepad/joystick input alongside keyboard/mouse.
 */
class Input : public Subsystem
{
	friend class Game;
public:
	class Axis
	{
		friend class Input;
	public:
		string name;
		uint16 positive;
		uint16 negative;
		float sensitivity;
		float dead;
		bool snap;
		/// @todo Joystick axis

		/// Number of the controller to use. 0 means all.
		byte joyNum = 0;


		float GetValue() const
		{
			return value;
		}

		Axis(string name="",
			 uint16 positive='\0',
			 uint16 negative='\0',
			 float sens=3.0f,
			 float dead=0.001f,
			 bool snap=true) :
			name(name), positive(positive), negative(negative), sensitivity(sens), dead(dead), snap(snap)
		{
		}

		~Axis()
		{
		}

	private:

		float value = 0.0f;

		bool posHeld = false;
		bool negHeld = false;
		bool down = false;
		bool up = false;
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
	Input(Game* game);
	~Input();

	virtual void Init();
	virtual void Update();

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop) const;

	void OnKeyPress(int key, int action, int mods);
	void OnCharPress(uint key);
	void OnMouseMove(vec2 pos);
	void OnMouseButton(int btn, int action, int mods);

	/// example:
	/// AddAxis({"Horizontal", 'w', 's'});
	void AddAxis(Axis axis);

	// Axes
	Axis* GetAxis(string name) const;

	// Buttons: Positive side of the given axis
	int8 GetButton(string name) const;
	int8 GetButtonDown(string name) const;
	int8 GetButtonUp(string name) const;
	
	int8 GetKeyDown(uint16 key) const;
	

#if !ANDROID
	ivec2 GetMousePos() const;
	void SetMousePos(ivec2 pos);
	void SetMousePos(int32 x, int32 y);
#else
	deque<Touch>& GetTouches();
	Touch& GetTouch(uint32 idx);
	void OnTouch(uint32 idx, int action, Touch data);
#endif

	void GetJoystickAxes();
	void OnCursorEnter(bool entered);
	void OnScroll(vec2 offset);

	bool lockMouse = false;
	float mouseSpeed = 1.0f;

protected:
	std::deque<Axis> axes;
	std::deque<Joystick> joysticks;
#if ANDROID
	std::deque<Touch> touches;
#endif

	/// The string of input this frame.
	string inputString;
	ivec2 mousePos;

	//bitset<sizeof(byte)> keysHeld;

private:
};

/** @} */
/** @} */
}
