/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// External

// Internal
#include <lfant/Subsystem.h>
#include <lfant/Range.h>

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

/**
 *
 *	\details
 *	@todo
 */
class SystemInfo : public Subsystem
{
public:

	struct HardDrive
	{
		string name;
		float total;
		float free;
	};

	struct Monitor
	{
		string name;
		u16vec2 resolution;
	};

	struct Processor
	{
		string name;
		uint16 cores;
	};

	SystemInfo(Game* game);
	virtual ~SystemInfo();

	virtual void Init();
	virtual void Destroy();

	string computerName;
	float memory;
	string OS;
	std::vector<HardDrive> drives;
	Processor cpu;
	string gpu;
	string username;
	Monitor monitor;
	Range<uint16> glVersion = { 0, 0 };

protected:

private:
};

/** @} */
/** @} */
}
