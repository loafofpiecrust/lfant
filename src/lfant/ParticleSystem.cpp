/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-08-02 by Taylor Snead
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

#include <lfant/ParticleSystem.h>

// External

// Internal
#include <lfant/Particle.h>
#include <lfant/Random.h>

#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>

namespace lfant
{

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init()
{
}

void ParticleSystem::Update()
{
	double delta;
	if(pausable)
	{
		delta = game->time->deltaTime;
	}
	else
	{
		delta = game->time->deltaTimeFixed;
	}

	if(startDelay > 0)
	{
		startDelay -= delta;
		return;
	}

	if(GetCount() > maxParticles)
	{
		return;
	}

	toEmit += rate / duration * delta;
	while(toEmit >= 1.0f)
	{
		if(recycle.size() > 0)
		{
			Emit(recycle[recycle.size() - 1]);
		}
		else
		{
			Emit();
		}
		toEmit -= 1.0f;
	}

	for(uint32_t i = 0; i < GetCount(); ++i)
	{
		if(particles[i]->active)
		{
			particles[i]->Update(delta);
		}
	}

	for(uint32_t i = 0; i < bursts.size(); ++i)
	{
		bursts[i].last += delta;
		if(bursts[i].last >= bursts[i].time)
		{
			for(uint i = 0; i < bursts[i].particles; ++i)
			{
				if(recycle.size() > 0)
				{
					Emit(recycle[recycle.size() - 1]);
				}
				else
				{
					Emit(1);
				}
			}
			bursts[i].last = 0;
		}
	}

}

void ParticleSystem::OnDestroy()
{
}

void ParticleSystem::Emit(uint32_t amount)
{
	for(uint32_t i = 0; i < amount; ++i)
	{
		Emit(new Particle());
	}
}

void ParticleSystem::Emit(Particle* pt, bool old)
{
	pt->system = this;
	pt->SetSizeRange(Random(size.start.min, size.start.max), Random(size.end.min, size.end.max));
	pt->SetGravity(gravity);
	pt->color.start = Random(color.start.min, color.start.max);
	pt->color.end = Random(color.end.min, color.end.max);
	pt->StartLife(Random(lifetime.start, lifetime.end));
	pt->Activate(true);
	GenerateVelocity(pt);
	pt->Init();
	particles.push_back(pt);

	if(old)
	{
		for(uint i = 0; i < recycle.size(); ++i)
		{
			if(recycle[i] == pt)
			{
				recycle.erase(recycle.begin() + i);
			}
		}
	}
}

void ParticleSystem::Recycle(Particle* pt)
{
	if(particles.size() + recycle.size() < maxParticles)
	{
		recycle.push_back(pt);
		pt->active = false;
	}
	else
	{
		pt->Destroy();
	}
}

void ParticleSystem::GenerateVelocity(Particle* pt)
{
	float speed = Random(this->speed.start.min, this->speed.start.max);
	vec3 dir;
	vec3 pos;

	// Cone emitter
	pt->transform->SetPosition( pos = vec3(Random(-radius, radius), 0.0f, Random(-radius, radius)) );
	dir = vec3(Random(-angle / 90, angle / 90), Random(0.0f, 1.0f), Random(-angle / 90, angle / 90));
	while(dir == vec3(0.0f))
	{
		// This is to prevent ever getting a 0 direction and generating an empty velocity.
		dir = vec3(Random(-angle / 90, angle / 90), Random(0.0f, 1.0f), Random(-angle / 90, angle / 90));
	}
	pt->velocity = dir * speed;

	// Sphere emitter
	pt->transform->SetPosition(vec3(0));
	dir = vec3(Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), Random(-1.0f, 1.0f));
	pt->transform->Translate(dir * Random(0.0f, radius));
	pt->velocity = dir * speed;

	// Straight direction emitter
	// Cube volume, radius being half the side length
	pt->transform->SetPosition(vec3(Random(-radius, radius), Random(-radius, radius), Random(-radius, radius)));
	// Square area
	pt->transform->SetPosition(vec3(Random(-radius, radius), 0.0f, Random(-radius, radius)));
	// Point emitter
	pt->transform->SetPosition(vec3(0));
	dir = vec3(0, 1, 0);
	pt->velocity = dir * speed;

	// Beginning and ending velocity range, for an interpolating velocity.
	pt->velRange.start = Random(velocity.start.min, velocity.start.max);
	pt->velRange.end = Random(velocity.end.min, velocity.end.max);

	//return pt->velocity;
}

uint32_t ParticleSystem::GetCount()
{
	return particles.size();
}

}
