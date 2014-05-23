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
#include <lfant/Game.h>

namespace lfant
{

Time::Time(Game* game) :
	Subsystem(game),
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
//	GetGame()->Log("Time::ResetTime: Started");
	startTime = hclock::now();
//	glfwSetTime(0.0);
//	GetGame()->Log("Time::ResetTime: Finished");
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

	GetGame()->Log("\n"+boost::lexical_cast<string>(GetDate()));

	ResetTime();
	UpdateTimes();
}

void Time::Update()
{
	UpdateTimes();

	for(auto i = timers.begin(); i != timers.end(); ++i)
	{
	//	GetGame()->Log("Timer '", timers[i]->name, "' updated at ", timers[i]->time);
		auto& t = *i;
		if(t.time <= 0.0f)
		{
			string name = t.name;
			timers.erase(i);
			--i;
		//	GetGame()->Log("Timer triggered");
			TriggerEvent(name);
			continue;
		}
		t.time -= deltaTime;
	}
}

void Time::SetTimer(string name, float time)
{
//	erase_all(name, " ");
//	name = type::Name(this) + "::" + name + "()";
	for(auto& t : timers)
	{
		if(t.name == name)
		{
			t.time = time;
			return;
		}
	}
	timers.emplace_back(name, time);
}

void Time::CancelTimer(string name)
{
//	erase_all(name, " ");
//	name = type::Name(this) + "::" + name + "()";
	for(uint i = 0; i < timers.size(); ++i)
	{
		if(timers[i].name == name)
		{
			timers.erase(timers.begin()+i);
			return;
		}
	}
}

float* Time::GetTimer(string name)
{
//	erase_all(name, " ");
//	name = type::Name(this) + "::" + name + "()";
	for(auto& t : timers)
	{
		if(t.name == name)
		{
			return &(t.time);
		}
	}
	return nullptr;
}


}
