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

// Internal
#include <lfant/ParticleSystem.h>
#include <lfant/Console.h>
#include <lfant/Time.h>
#include <lfant/Game.h>

// External

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
//	SetParamDiffs();
}

void Particle::SetPosition(vec3 pos)
{
	position = pos;
//	system->UpdatePosition(this);
}

static vec3 zeroVector = vec3(0);

void Particle::Update()
{
//	Log("Updating a particle");
	InterpParams();

//	if(velocity != zeroVector)
	{
		SetPosition(position + velocity * game->time->deltaTime);
	}

	lifetime -= game->time->deltaTime;
//	if(lifetime <= 0.0f)
	{
	//	system->Recycle(this);
	}
}

void Particle::SetParamDiffs(vec3 vel, vec4 col, float siz)
{
	sizeDiff = 0.0f;
	colorDiff = vec4(0);
	velocityDiff = vec3(0.0f);

	if(siz != size)
	{
		sizeDiff = (siz - size) / lifetime;
	}
	if(col != color)
	{
		colorDiff = (col - color) / lifetime;
	}
	if(vel != velocity)
	{
		velocityDiff = (vel - velocity) / lifetime;
	}
}

void Particle::InterpParams()
{
//	if(colorDiff != vec4(0))
	{
		color += colorDiff * game->time->deltaTime;
		// Apply color.
	}
//	if(sizeDiff != 0.0f)
	{
		size += sizeDiff * game->time->deltaTime;
	}
//	if(velocityDiff != vec3(0.0f))
	{
		//ApplyForce(-velocity.x);
		velocity += velocityDiff * game->time->deltaTime;
		//ApplyForce(velocity.x);
	}
//	system->UpdatePosition(this);
}

float Particle::GetSize()
{
	return size;
}

vec4 Particle::GetColor()
{
	return color;
}

void Particle::StartLife(float life)
{
	lifetime = life;
//	age = 0.0f;
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

float Particle::GetSpeed()
{
	return length(velocity);
}

void Particle::ApplyForce(vec3 force)
{
	velocity += force * game->time->deltaTime;
}

void Particle::ApplyForce(float speed, vec3 dir)
{
	velocity += dir * speed;
}

}
