/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-08-05 by Taylor Snead
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

// Internal
#include "Subsystem.hpp"
#include "Range.hpp"

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

/**
 *
 *	@details
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
		vec2 resolution;
	};

	struct Processor
	{
		string name;
		uint16_t cores;
	};

	SystemInfo();
	virtual ~SystemInfo();

	virtual void Init();

	string computerName;
	float memory;
	string OS;
	vector<HardDrive> drives;
	Processor cpu;
	string gpu;
	string username;
	Monitor monitor;
	Range<uint16_t> glVersion = { 0, 0 };

protected:

private:
};

/** @} */
/** @} */
}
