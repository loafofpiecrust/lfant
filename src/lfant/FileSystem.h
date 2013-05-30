/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-08-08 by Taylor Snead
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
#pragma once

#include <lfant/stdafx.h>

// External

// Internal
#include <lfant/util/Path.h>
#include <lfant/Subsystem.h>


namespace lfant
{
/** @addtogroup Game
 *	@{
 */
/** @addtogroup Utilities
 *	@{
 */

/**
 *
 */
class FileSystem : public Subsystem
{
	friend class Settings;
public:
	FileSystem();
	~FileSystem();

	void Init();

	path GetUserPath(string name);
	path GetGamePath(string name);

	deque<path> GetGameFiles(string dir);

protected:

public:
	string gameFolder = "../..";
	string userFolder;
};

/** @} */
/** @} */
}
