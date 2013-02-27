/******************************************************************************
 *
 * ShadowFox Engine Source
 * Copyright (C) 2012-2013, by ShadowFox Studios
 * Created: 2012-08-13 by Taylor Snead
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/


#include <lfant/Audio.hpp>

// External
#include <fmod.hpp>

// Internal

#include <lfant/Console.hpp>

namespace lfant
{

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
	FMOD::System_Create(&soundSystem);
	soundSystem->init(maxChannels, FMOD_INIT_NORMAL, 0);
}

void Audio::Update()
{

}

void Audio::OnDestroy()
{
}

FMOD::Sound* Audio::PlaySound(string file, bool loop)
{
	FMOD::Sound* snd;
	soundSystem->createSound(file.c_str(), FMOD_SOFTWARE | FMOD_2D, 0, &snd);
	if(loop)
	{
		snd->setMode(FMOD_LOOP_NORMAL);
	}
	return snd;
}

FMOD::Sound* Audio::PlaySound3d(string file, vec3 position, bool loop)
{
}

}
