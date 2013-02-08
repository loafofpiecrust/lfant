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

// External

// Internal

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Subsystems
 *	 @{
 */

/**	Manages game settings from a config file for individual users.
 *	@details
 *		Description
 *	@todo
 *		Maybe combine with a ResourceManager that handles file paths, as LoadProfile and SaveProfile;
 *			or keep all that stuff inside this class?
 */
class Settings : public Subsystem
{
public:
	Settings();
	~Settings();
};

/** @} */
/** @} */
}
