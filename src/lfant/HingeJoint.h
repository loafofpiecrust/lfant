/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-11-02 by Taylor Snead
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
#include <lfant/Joint.h>

namespace lfant
{
/**	@addtogroup Game
 *	@{
 */
/** @addtogroup Physics
 *	@{
 */

/**
 *
 *	\details
 *	@todo
 */
class HingeJoint : public Joint
{
	//	friend class Physics;
public:
	using Joint::~Joint;

	struct SpringInfo
	{
		bool use;
		float force;
		float damping;
	};

	struct Limits
	{
		bool use;
		float min;
		float max;
		float bounceMin;
		float bounceMax;
	};

	vec3 anchor;
	vec3 axis;
	SpringInfo spring;
	Limits limits;
	float breakForce;

protected:
	using Joint::Joint;

private:
	virtual btTypedConstraint* GetConstraint();
	virtual void SetConstraint(btTypedConstraint* con);

};

/** @} */
/** @} */
}
