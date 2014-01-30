/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013, by LazyFox Studios
*	Created: 2012-08-13 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
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
#include <lfant/Component.h>

// External

namespace lfant {

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

		Clip(string name, uint16_t start, uint16_t end, uint16_t fps) :
			start(start), end(end), frameRate(fps)
		{
		}

		virtual void Save(Properties* prop) const;
		virtual void Load(Properties* prop);

		string name = "";
		uint16_t start = 1;
		uint16_t end = 2;
		uint16_t frameRate = 15;
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
	deque<Clip*> clips;

private:
};

}
