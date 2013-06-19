/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2013-06-08 by Taylor Snead
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

namespace lfant {
namespace galaga {

/** @addtogroup Galaga
 *	@{
 */
/** @addtogroup Entities
 *	@{
 */

/**
 *
 */
class MouseLook : public lfant::Component
{
	DECLARE_COMP(MouseLook)
public:
	MouseLook();
	~MouseLook();

	void Save(Properties* prop);
	void Load(Properties* prop);

	void Init();
	void OnSetMousePos(ivec2 pos);
	void Look(vec3 rot);

protected:
	float lookSpeed = 0.01f;
	ivec2 lastMouse = ivec2(0);

private:

};

/// @}
/// @}
}
}
