/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include "ParticleSystem.hpp"

// External

// Internal
#include "Particle.hpp"
#include "Random.hpp"
#include "Engine.hpp"
#include "Time.hpp"
#include "Renderer.hpp"

namespace sfs
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

	game->renderer->RenderParticles(this);
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
	game->renderer->AddParticles(this);
}

void ParticleSystem::Emit(Particle* pt, bool old)
{
	pt->system = this;
	pt->SetSizeRange(random(size.start.min, size.start.max), random(size.end.min, size.end.max));
	pt->SetGravity(gravity);
	pt->color.start = random(color.start.min, color.start.max);
	pt->color.end = random(color.end.min, color.end.max);
	pt->StartLife(random(lifetime.start, lifetime.end));
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
	game->renderer->AddParticles(this);
}

void ParticleSystem::GenerateVelocity(Particle* pt)
{
	float speed = random(this->speed.start.min, this->speed.start.max);
	vec3 dir;
	vec3 pos;

	// Cone emitter
	pt->transform->position = pos = vec3(random(-radius, radius), 0.0f, random(-radius, radius));
	dir = vec3(random(-angle / 90, angle / 90), random(0.0f, 1.0f), random(-angle / 90, angle / 90));
	while(dir == vec3(0.0f))
	{
		// This is to prevent ever getting a 0 direction and generating an empty velocity.
		dir = vec3(random(-angle / 90, angle / 90), random(0.0f, 1.0f), random(-angle / 90, angle / 90));
	}
	pt->velocity = dir * speed;

	// Sphere emitter
	pt->transform->position = vec3(0.0f);
	dir = vec3(random(-1.0f, 1.0f), random(-1.0f, 1.0f), random(-1.0f, 1.0f));
	pt->transform->Translate(dir * random(0.0f, radius));
	pt->velocity = dir * speed;

	// Straight direction emitter
	// Cube volume, radius being half the side length
	pt->transform->position = vec3(random(-radius, radius), random(-radius, radius), random(-radius, radius));
	// Square area
	pt->transform->position = vec3(random(-radius, radius), 0.0f, random(-radius, radius));
	// Point emitter
	pt->transform->position = vec3(0.0f);
	dir = vec3(0.0f, 1.0f, 0.0f);
	pt->velocity = dir * speed;

	// Beginning and ending velocity range, for an interpolating velocity.
	pt->velRange.start = random(velocity.start.min, velocity.start.max);
	pt->velRange.end = random(velocity.end.min, velocity.end.max);

	//return pt->velocity;
}

uint32_t ParticleSystem::GetCount()
{
	return particles.size();
}

}
