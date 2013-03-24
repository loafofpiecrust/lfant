/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-26 by Taylor Snead
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

#include <lfant/Component.h>

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Physics
 *	 @{
 */

/**
 *	This class adds Magnet capabilities to the Owner.
 *		Allows attraction and repulsion based on the position and rotation
 *		of two magnetic Entities.
 *	@todo
 *		Actually figure this out.
 *		Efficient handling of Magnet to Magnet interaction.
 *		Using rotation and position to roughly guess how much force to use.
 */
class Magnet : public Component
{
public:
	enum class Mode : byte
	{
		None = 0,
		Attract,
		Repulse,
		Both = Attract | Repulse
	};

	Magnet();
	~Magnet();

protected:

private:
	/// The strength and range of attraction and repulsion to use.
	float magnetism;

	/// The mode of magnetism. 0 = none. 1 = attraction. 2 = repulsion. 3 = both.
	Mode mode;

	/// The axis at which to split the poles. 1 = x; 2 = y; 3 = z.
	byte axis;
};

/** @} */
/** @} */
}
