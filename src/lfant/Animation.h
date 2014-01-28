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
		Clip(string name, uint16_t start, uint16_t end, uint16_t fps) :
			start(start), end(end), fps(fps)
		{
		}

		string name = "";
		uint16_t start = 1;
		uint16_t end = 2;
		uint16_t fps = 15;
	};

	virtual void Save(Properties* prop) const;
	virtual void Load(Properties* prop);

	virtual void Play(string anim = "") = 0;
	virtual void Stop(string anim = "") = 0;

	virtual void AddClip(string name, uint16_t start, uint16_t end, uint16_t fps);

	Clip* GetClip(string name);

protected:
	string currentClip = "";
	deque<Clip> clips;

private:
};

}
