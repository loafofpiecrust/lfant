/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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

#include <lfant/stdafx.h>

// External

// Internal
#include <lfant/Component.h>
#include <lfant/Range.h>
#include <lfant/Sound.h>

namespace lfant
{
/** @addtogroup Game
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
//	DECLARE_TYPE(Component, AudioSource)
public:
	AudioSource();
	virtual ~AudioSource();

	virtual void Init();

	Sound* AddSound(string name);
	Sound* GetSound(string name);

protected:

	void OnSetPosition();
	void OnSetRotation();
	void OnSetVelocity(vec3 vel);

	float pitch;
	float volume;
	Range<float> distanceLimits;

	deque<Sound> sounds;

private:
	string inputFile = "";
};
}
