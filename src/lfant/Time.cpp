/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-21 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#include <lfant/Time.h>

// External
#include <boost/chrono.hpp>
//#include <GLFW/glfw3.h>

// Internal
#include <lfant/Console.h>

namespace lfant
{

Time::Time() :
	timeScale(1.0f),
	dateTime(boost::posix_time::second_clock::local_time())
{
	//Init();
}

Time::~Time()
{
}

double Time::GetTime()
{
	return boost::chrono::duration<double>(hclock::now() - startTime).count();
//	return glfwGetTime();
}

boost::posix_time::ptime& Time::GetDate()
{
	return dateTime;
}

void Time::ResetTime()
{
	// Figure something out?
//	Log("Time::ResetTime: Started");
	startTime = hclock::now();
//	glfwSetTime(0.0);
//	Log("Time::ResetTime: Finished");
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
	Subsystem::Init();

	Log("\n"+boost::lexical_cast<string>(GetDate()));
	
	ResetTime();
	UpdateTimes();
}

void Time::Update()
{
	UpdateTimes();
}

}
