/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-22 by Taylor Snead
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
#include "stdafx.hpp"

// External

// Internal
#include "Component.hpp"
#include "Entity.hpp"
#include "Engine.hpp"
#include "Time.hpp"

namespace sfs
{
	/** @addtogroup Engine
	 *	 @{
	 */
	/** @addtogroup Components
	 *	 @{
	 */

	/**	Spawns a given Entity type at a certain location and rotation.
	 *	@details
	 *		Description
	 *	@todo
	 *		Todo
	 */
	template<class T>
	class Spawner : Component
	{
	public:
		Spawner()
		{
		}
		~Spawner()
		{
		}

		void Spawn()
		{
			char* str;
			sprintf(str, "%u", count);
			Entity::Spawn(baseName + string(str));
			++count;
		}

		virtual void Update()
		{
			if(delay <= 0.0f)
			{
				Spawn();
				enabled = false;
				return;
			}

			float time = game->time->GetTime();
			if(time >= lastSpawn + delay)
			{
				// If we've passed the delay, spawn the next one.
				Spawn();
				// Reset the last spawn time
				lastSpawn = time;
				// Check if we've spawned the amount
				if(amount > 0 && count >= amount)
				{
					enabled = false;
					return;
				}
			}
		}

		/// The delay between each spawn. If 0, the point will only spawn once.
		float delay = 0.0f;

		/// The basis name for each spawned Entity. Will append the count to this.
		string baseName = "entity";

		/// The amount to spawn. If 0, it will spawn forever.
		uint32_t amount = 0;

	protected:

	private:

		/// Time of the last spawn. Used for timing each spawn correctly.
		float lastSpawn = 0.0f;

		/// Amount spawned so far.
		uint32_t count = 0;
	};

	/** @} */
	/** @} */
}
