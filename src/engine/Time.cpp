/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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
#include "Time.hpp"

// External
#include <boost/chrono/chrono.hpp>

// Internal

using namespace sfs;

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
	using namespace boost::chrono;
	return duration<double>(sfclock::now() - startTime).count();
}

void Time::ResetTime()
{
	// Figure something out?
	startTime = sfclock::now();
}

void Time::UpdateTimes()
{
	deltaTimeFixed = GetTime() - lastFrame;
	deltaTime = deltaTimeFixed * timeScale;
	lastFrame = GetTime();
	frameRate = 1 / deltaTimeFixed;
}

void Time::Init()
{
	ResetTime();
	UpdateTimes();
}

void Time::Update()
{
	UpdateTimes();
}
