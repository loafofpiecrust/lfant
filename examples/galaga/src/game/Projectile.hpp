/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-28 by Taylor Snead
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

// External

// Internal
#include <Entity.hpp>

namespace sfs
{
	struct KillParams
	{

	};

	/** @addtogroup Game
	 *	 @{
	 */
	/** @addtogroup Entities
	 *	 @{
	 */

	/**	Projectile
	 *	@details
	 *		Description
	 *	@todo
	 *		Todo
	 */
	class Projectile : public Entity
	{
	public:
		Projectile();
		~Projectile();

		virtual void Init();

		void Impact( KillParams death );
		void Penetrate( float thickness );
		void Launch( const vec3 pos, const vec3 dir, const vec3 velocity, float speedScale = 1.0f );
		void Ricochet();

	protected:

	private:
	};

/** @} */
/** @} */
}
