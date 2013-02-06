/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-08-02 by Taylor Snead
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
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btScalar.h>

// Internal

namespace sfs
{
	/** @addtogroup Engine
	 *	@{
	 */
	/** @addtogroup Subsystems
	 *	@{
	 */

	/**	My custom DynamicsWorld class, adding new functionality.
	 *	@details
	 *		Adds some new functionality, such as point-based gravity allowing
	 *		you to create realistic solar systems, or just points that pull
	 *		you in. More soon...
	 *	@todo
	 *		Todo
	 */
	class sfDynamicsWorld : public btDiscreteDynamicsWorld
	{
	public:
		sfDynamicsWorld(btDispatcher* dispatcher, btBroadphaseInterface* pairCache, btConstraintSolver* constraintSolver,
						btCollisionConfiguration* collisionConfiguration);
		virtual ~sfDynamicsWorld();

		/// Apply all gravity as points, disabling normal gravity.
		virtual void applyGravity();

	protected:

	private:
	};

	/** @} */
	/** @} */
}
