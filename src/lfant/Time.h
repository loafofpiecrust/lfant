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
*		http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/
#pragma once

#include <lfant/stdafx.h>

// External
#include <boost/chrono.hpp>

// Internal
#include <lfant/util/qumap.h>
#include <lfant/Subsystem.h>

using namespace boost::chrono;

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

typedef boost::chrono::high_resolution_clock hclock;

/**
 *
 */
class Time : public Subsystem
{
public:
	Time();
	~Time();

	virtual void Init();
	virtual void Update();

	/// Gets the amount of seconds since Game::Init was called.
	double GetTime();
	void ResetTime();
	void UpdateTimes();

	/** Sets a timer using a name, length, and optionally a callback function. Shouldn't be used for long time periods (1 hour+).
	 *	@param name The name of this timer for retrieval before finish.
	 *	@param length The amount of time this timer should wait before finishing.
	 *	@param callback The function to call when the timer ends. Should be set with boost::bind(&func, &inst);
	 */
	void SetTimer(string name, float length);

	float GetTimer(string name);

	void CallTimer(string name);

	// Properties

	/// Deltatime in seconds
	float deltaTime = 0.016666667f;
	float deltaTimeFixed = 0.016666667f;
	float frameRate = 60.0f;
	float timeScale = 1.0f;
	double lastFrame;

private:
	qumap<string, float> timers;
	hclock::time_point startTime;
};

/** @} */
/** @} */
}
