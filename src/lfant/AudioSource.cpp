/*
*	Copyright (C) 2013-2014, by loafofpiecrust
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
//#include <plaid/audio.h>

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
}

Sound* AudioSource::GetSound(string name)
{
}

void AudioSource::OnSetPosition()
{
}

void AudioSource::OnSetRotation()
{
}

void AudioSource::OnSetVelocity(vec3 vel)
{
}

}
