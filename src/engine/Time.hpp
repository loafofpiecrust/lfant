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
#include "stdafx.hpp"

// External
#include <boost/chrono.hpp>

// Internal
#include "Subsystem.hpp"

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Subsystems
 *	 @{
 */

struct STimer
{
	string name;
	double length;
	boost::function<void()> callback;
};

typedef boost::chrono::high_resolution_clock sfclock;

/**	Timer
 *	@details
 *		Description
 *	@todo
 *		Todo
 */
class Time : public Subsystem
{
public:
	Time();
	~Time();

	virtual void Init();
	virtual void Update();

	/// Gets the amount of seconds since Engine::Init was called.
	double GetTime();
	void ResetTime();
	void UpdateTimes();

	/** Sets a timer using a name, length, and optionally a callback function. Shouldn't be used for long time periods (1 hour+).
	 *	@param name The name of this timer for retrieval before finish.
	 *	@param length The amount of time this timer should wait before finishing.
	 *	@param callback The function to call when the timer ends. Should be set with boost::bind(&func, &inst);
	 */
	STimer& SetTimer(string name, const double length, boost::function<void()> callback /*= nullptr*/);

	STimer& GetTimer(string name);

	void CallTimer(STimer& timer);
	void CallTimer(string name);
	void CallTimer(int idx);

	/// This function is called on a new thread to handle a timer efficiently.
	void TimerThread(STimer& tmr);

	// Properties

	/// Deltatime in seconds
	double deltaTime;
	double deltaTimeFixed;
	double frameRate;
	double timeScale;

private:
	double lastFrame;
	vector<STimer> timers;
	sfclock::time_point startTime;
};

/** @} */
/** @} */
}
