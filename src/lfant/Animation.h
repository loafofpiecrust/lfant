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
#include <lfant/Component.h>

// External

namespace lfant
{

class Animation : public Component
{
public:

	class Clip
	{
	public:
		enum class Mode : byte
		{
			Loop = 0,
			Bounce,
			Once,
			Default = Loop
		};

		Clip()
		{
		}

		Clip(string name, uint16 start, uint16 end, uint16 fps) :
			start(start), end(end), frameRate(fps)
		{
		}

		virtual void Save(Properties* prop) const;
		virtual void Load(Properties* prop);

		string name = "";
		uint16 start = 1;
		uint16 end = 2;
		uint16 frameRate = 15;
		Mode mode = Mode::Default;
	};

	virtual void Save(Properties* prop) const;
	virtual void Load(Properties* prop);

	virtual void SetCurrent(string name) {}

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;

	virtual void AddClip(Clip* clip);

	Clip* GetClip(string name);

protected:
	float currentTime = 0.0f;
	string currentClip = "";
	std::deque<Clip*> clips;

private:
};

}
