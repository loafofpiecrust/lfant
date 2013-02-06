/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013, by ShadowFox Studios
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

#include "Audio.hpp"

// External

// Internal
#include "Console.hpp"

using namespace sfs;

Audio::Audio()
{
	//ctor
}

Audio::~Audio()
{
	//dtor
}

void Audio::Init()
{
	/*
	 //int error;
	 if (!alutInitWithoutContext(0, 0))
	 {
	 //error = alutGetError();
	 //Log("Audio: There is an ALUT error: ", alutGetErrorString(alutGetError()));
	 }

	 SetDevice(alcOpenDevice(0));
	 if (!GetDevice())
	 {
	 //	Log("Audio: Couldn't create the OpenAL audio device.");
	 }

	 context = alcCreateContext(device, 0);
	 if (!GetContext())
	 {
	 //	Log("Audio: Couldn't create the OpenAL audio context.");
	 }

	 alcMakeContextCurrent(context);

	 alListener3f(AL_POSITION, 0, 0, 0);
	 alListener3f(AL_VELOCITY, 0, 0, 0);
	 alListener3f(AL_ORIENTATION, 0, 0, -1);

	 alGenSources(1, &source);

	 alSourcef(source, AL_PITCH, 1);
	 alSourcef(source, AL_GAIN, 1);
	 alSource3f(source, AL_POSITION, 0, 0, 0);
	 alSource3f(source, AL_VELOCITY, 0, 0, 0);
	 alSourcei(source, AL_LOOPING, AL_FALSE);

	 //alGenBuffers(1, &mBuffer);
	 //alutLoadMemoryWaveform(,)
	 */
}

void Audio::Update()
{

}
