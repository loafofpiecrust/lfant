/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
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
