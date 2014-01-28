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

#include <lfant/AudioSource.h>

// External
#include <AL/al.h>
#include <AL/alc.h>

// Internal
#include <lfant/Transform.h>
#include <lfant/Audio.h>

namespace lfant
{

//IMPLEMENT_TYPE(Component, AudioSource)

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
	ConnectEvent(SENDER(owner, OnSetPosition), RECEIVER(this, OnSetPosition));
	ConnectEvent(SENDER(owner, OnSetRotation), RECEIVER(this, OnSetRotation));
	ConnectEvent(SENDER(owner, OnSetVelocity), RECEIVER(this, OnSetVelocity));
}

Sound* AudioSource::AddSound(string name)
{
	sounds.emplace_back(this, name);
	return &sounds[sounds.size()-1];
}

Sound* AudioSource::GetSound(string name)
{
	for(auto& snd : sounds)
	{
		if(snd.name == name)
		{
			return &snd;
		}
	}
	return nullptr;
}

void AudioSource::OnSetPosition()
{
	vec3 pos = owner->transform->GetWorldPosition();
	for(auto& snd : sounds)
	{
		alSource3f(snd.id, AL_POSITION, pos.x, pos.y, pos.z);
	}
}

void AudioSource::OnSetRotation()
{
	vec3 dir = owner->transform->GetDirection();
	for(auto& snd : sounds)
	{
		alSource3f(snd.id, AL_DIRECTION, dir.x, dir.y, dir.z);
	}
}

void AudioSource::OnSetVelocity(vec3 vel)
{
	for(auto& snd : sounds)
	{
		alSource3f(snd.id, AL_VELOCITY, vel.x, vel.y, vel.z);
	}
}

}
