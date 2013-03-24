/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-08-04 by Taylor Snead
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

#include <lfant/Particle.h>

// External

// Internal
#include <lfant/ParticleSystem.h>

namespace lfant
{

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::Init()
{
	//ApplyForce( speed, transform.GetDir() );
	SetParamDiffs();
}

void Particle::Update(double delta)
{
	age += delta;
	InterpParams(delta);

	if(gravity != vec3(0, 0, 0))
	{
		ApplyForce(gravity);
	}
	if(velocity != vec3(0, 0, 0))
	{
		transform->Translate(velocity * (float)delta + velRange.min);
	}
	if(age >= lifetime)
	{
		system->Recycle(this);
	}
}

void Particle::SetParamDiffs()
{
	sizeDiff = 0.0f;
	colorDiff = rgba(0);
	velDiff = vec3(0.0f);

	if(size.max > size.min)
	{
		sizeDiff = (size.max - size.min) / lifetime;
	}
	if(color.max != color.min)
	{
		colorDiff = (color.max - color.min) / lifetime;
	}
	if(velRange.max != velRange.min)
	{
		velDiff = (velRange.max - velRange.min) / lifetime;
	}
}

void Particle::InterpParams(double delta)
{
	if(colorDiff != rgba(0) && color.min != color.max)
	{
		color.min += colorDiff * delta;
		// Apply color.
	}
	if(sizeDiff != 0.0f && size.min != size.max)
	{
		size.min += sizeDiff * delta;
		transform->SetScale(vec3(size.min));
	}
	if(velDiff != vec3(0.0f) && velRange.min != velRange.max)
	{
		//ApplyForce(-velRange.x);
		velRange.min += velDiff * vec3(delta);
		//ApplyForce(velRange.x);
	}
}

void Particle::StartLife(float life)
{
	lifetime = life;
	age = 0.0f;
	Init();
}

void Particle::Activate(bool reset /* =false */)
{
	if(reset)
	{
		// Do something that's a resetter, I dunno...
	}
	active = true;
	// Turn on rendering...
}

void Particle::Deactivate()
{
	// This should stop updating?
	active = false;
	// Turn off rendering...
	// Stop Updating...
}

void Particle::Destroy()
{
	// Stops drawing
	Deactivate();
}

void Particle::SetSpeed(float speed)
{
	this->speed = speed;
}

void Particle::ApplyForce(vec3 force)
{
	velocity += force;
}

void Particle::ApplyForce(float speed, vec3 dir)
{
	velocity += dir * speed;
}

}
