/******************************************************************************
*
* LFANT Source
* Copyright (C) 2012-2013, by LazyFox Studios
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


#include <lfant/Audio.h>

// Internal
#include <lfant/Console.h>

// External
//#include <bass.h>
#include <AL/al.h>
#include <AL/alc.h>

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
	Subsystem::Init();

	device = alcOpenDevice(0);
	if(!device)
	{
		// failed to create
	}

	context = alcCreateContext(device, 0);
	alcMakeContextCurrent(context);
	if(!context)
	{
		// failed to create
	}

	
}

void Audio::Update()
{

}

void Audio::Deinit()
{
}

}
