/******************************************************************************
*
*	ShadowFox Engine Source
*	Copyright (C) 2012-2013 by ShadowFox Studios
*	Created: 2013-02-07 by Taylor Snead
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

#include <lfant/stdafx.hpp>

// External

// Internal
#include "lfant/Component.hpp"
#include "lfant/Range.hpp"

namespace FMOD
{
class Sound;
class Channel;
}

namespace lfant
{
/** @addtogroup Engine
 *	@{
 */
/** @addtogroup Audio
 *	@{
 */

/**
 *
 */
class AudioSource : public Component
{
public:
	AudioSource();
	virtual ~AudioSource();

	virtual void Init();

	virtual void Play();
	virtual void Pause();
	virtual void Stop();
	virtual void Restart();

	void SetSource(string file);

	void SetVolume(float volume);

protected:

	void OnSetPosition();

	uint32 sourceId = 0;

	FMOD::Sound* sound;
	FMOD::Channel* channel;
	float volume;
	Range<float> distanceLimits;

private:
	string inputFile = "";
};
}
