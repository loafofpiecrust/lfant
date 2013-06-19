/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-08-03 by Taylor Snead
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
#include <lfant/Transform.h>
#include <lfant/Range.h>

namespace lfant
{
class ParticleSystem;

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Particles
 *	 @{
 */

/**
 *	Particle
 */
class Particle
{
public:
	Particle();
	virtual ~Particle();

	// Make these virtual when they need to be inherited and overwritten.
	void Init();
	void Update();
	void Destroy();

	void SetPosition(vec3 pos);

	float GetSize();
	vec4 GetColor();

	//void Launch();

	void Activate(bool reset = false);
	void Deactivate();
	void SetVelocity(vec3 vel);
	void SetSizeRange(float start, float end)
	{
		size.min = start;
		size.max = end;
	}
	void StartLife(float life);
	void InterpParams();
	void SetParamDiffs();
	void ApplyForce(vec3 force);
	void ApplyForce(float speed, vec3 dir);

	/// Converts the velocity vector into a speed float.
	float GetSpeed();

	Range<float> size;
	Range<vec3> velocity;
	Range<vec4> color;

	float sizeDiff;
	vec3 velocityDiff;
	vec4 colorDiff;

//	vec3 gravity;
	float lifetime = 1.0f;
	bool active = true;

	vec3 position = vec3(0);
//	float size = 1;

//	ParticleSystem* system;

protected:

private:
};

/** @} */
/** @} */
}
