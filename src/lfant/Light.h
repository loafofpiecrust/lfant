/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-01 by Taylor Snead
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
/** @addtogroup Rendering
 *	 @{
 */

/**
 *
 */
class Light : public Component
{
public:

	/**
	 *	Supported light types.
	 */
	enum class Type : byte
	{
		Directional = 1,
		Point = 2,
		Spot = 3
	};

	Light();
	~Light();

	virtual void Init();
	virtual void Update();

protected:

	struct Directional
	{
	};

	struct Point
	{
		float range;
	};

	struct Spot
	{
		float range;
		float innerAngle;
		float outerAngle;
	};

	union
	{
		Directional* directional;
		Point* point;
		Spot* spot;
	};

	vec3 color;
	Type type;

private:

};

/** @} */
/** @} */
}
