/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-07-21 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#include <lfant/Time.h>

// External
#include <boost/chrono.hpp>

// Internal

#include <lfant/Console.h>

namespace lfant
{

Time::Time() :
	timeScale(1.0f)
{
	//Init();
}

Time::~Time()
{
}

double Time::GetTime()
{
	return boost::chrono::duration<double>(hclock::now() - startTime).count();
}

void Time::ResetTime()
{
	// Figure something out?
	Log("Time::ResetTime: Started");
	startTime = hclock::now();
	Log("Time::ResetTime: Finished");
}

void Time::UpdateTimes()
{
	deltaTimeFixed = GetTime() - lastFrame;
	deltaTime = deltaTimeFixed * timeScale;
	lastFrame = GetTime();
	frameRate = 1 / deltaTimeFixed;
}

void Time::SetTimer(string name, float length)
{
	timers[name] = length;
}

float Time::GetTimer(string name)
{
	return timers[name];
}

void Time::Init()
{
	ResetTime();
	UpdateTimes();
}

void Time::Update()
{
	UpdateTimes();

	for(auto& t : timers)
	{
		if(t.second > 0.0f)
		{
			t.second -= deltaTime;
		}
		else
		{
			Trigger(t.first);
			timers.erase(t.first);
		}
	}
}

}
