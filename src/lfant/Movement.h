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
class Rigidbody;

/** @addtogroup Galaga
 *	@{
 */
/** @addtogroup Entities
 *	@{
 */

/**
 *
 */
class Movement : public lfant::Component
{
public:
	Movement();
	~Movement();

	virtual void Save(lfant::Properties* prop) const;
	virtual void Load(lfant::Properties* prop);

	virtual void Init();
	virtual void Update();
	virtual void FixedUpdate();

	void Move(vec3 velocity);

protected:
	lfant::Rigidbody* rigidbody = nullptr;
	float movementSpeed = 1.0f; // m/s
	bool usePhysics = true;


private:

};

/// @}
/// @}
}
