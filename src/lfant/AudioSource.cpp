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

#include <lfant/AudioSource.hpp>

// External
#include <fmod.hpp>

// Internal

#include <lfant/Audio.hpp>

namespace lfant
{

AudioSource::AudioSource()
{
	//ctor
}

AudioSource::~AudioSource()
{
	//dtor
}

void AudioSource::Init()
{
	if(inputFile != "")
	{
		SetSource(inputFile);
	}
	Connect(SENDER(owner->transform, SetPosition), RECEIVER(this, OnSetPosition));
}

void AudioSource::SetSource(string file)
{
//	game->audio->soundSystem->createSound(file.c_str(), FMOD_SOFTWARE | FMOD_3D, 0, &sound);
//	sound->set3DMinMaxDistance(distanceLimits.min, distanceLimits.max);

}

void AudioSource::Play()
{
}

void AudioSource::Pause()
{
}

void AudioSource::Stop()
{
}

void AudioSource::Restart()
{
}

void AudioSource::OnSetPosition()
{

}

void AudioSource::SetVolume(float volume)
{
	this->volume = volume;
}

}
