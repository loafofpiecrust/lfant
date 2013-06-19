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
	SetParamDiffs();
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

	if(velocity.start != zeroVector)
	{
		SetPosition(position + velocity.start * game->time->deltaTime);
	}

	lifetime -= game->time->deltaTime;
//	if(lifetime <= 0.0f)
	{
	//	system->Recycle(this);
	}
}

void Particle::SetParamDiffs()
{
	sizeDiff = 0.0f;
	colorDiff = vec4(0);
	velocityDiff = vec3(0.0f);

	if(size.end != size.start)
	{
		sizeDiff = (size.end - size.start) / lifetime;
	}
	if(color.end != color.start)
	{
		colorDiff = (color.end - color.start) / lifetime;
	}
	if(velocity.end != velocity.start)
	{
		velocityDiff = (velocity.end - velocity.start) / lifetime;
	}
}

void Particle::InterpParams()
{
	if(colorDiff != vec4(0) && color.start != color.end)
	{
		color.start += colorDiff * game->time->deltaTime;
		// Apply color.
	}
	if(sizeDiff != 0.0f && size.start != size.end)
	{
		size.start += sizeDiff * game->time->deltaTime;
	}
	if(velocityDiff != vec3(0.0f) && velocity.start != velocity.end)
	{
		//ApplyForce(-velocity.x);
		velocity.start += velocityDiff * game->time->deltaTime;
		//ApplyForce(velocity.x);
	}
//	system->UpdatePosition(this);
}

float Particle::GetSize()
{
	return size.start;
}

vec4 Particle::GetColor()
{
	return color.start;
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
	return length(velocity.start);
}

void Particle::ApplyForce(vec3 force)
{
	velocity.start += force * game->time->deltaTime;
}

void Particle::ApplyForce(float speed, vec3 dir)
{
	velocity.start += dir * speed;
}

}
