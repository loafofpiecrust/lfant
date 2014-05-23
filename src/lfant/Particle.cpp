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

void Particle::Update(float delta)
{
//	if(colorDiff != vec4(0))
	{
		color += colorDiff * delta;
	//	color.a = 1.0f;
		// Apply color.
	}
//	if(sizeDiff != 0.0f)
	{
		size += sizeDiff * delta;
	}
//	if(velocityDiff != vec3(0.0f))
	{
		//ApplyForce(-velocity.x);
		velocity += velocityDiff * delta;
		//ApplyForce(velocity.x);
	}
//	system->UpdatePosition(this);

//	if(velocity != zeroVector)
	{
		position += velocity * delta;
	//	std::cout << "new particle position: " << lexical_cast<string>(position) << "\n";
	}

	lifetime -= delta;
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
	//	std::cout << "vel" << lexical_cast<string>(vel) << ", velocity" << lexical_cast<string>(velocity) << "lifetime: " << lifetime << "\n";
		velocityDiff = (vel - velocity) / lifetime;
	//	std::cout << "Initing new particle with vel" << lexical_cast<string>(velocityDiff) << "\n";
	}
	
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

void Particle::ApplyForce(vec3 dir, float speed)
{
	velocity += dir * speed;
}

}
