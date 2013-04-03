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

	for(auto& particle : particles)
	{
		if(particle->active)
		{
			particle->Update(delta);
		}
		else if(toEmit >= 1.0f)
		{
			Emit(particle, true);
			toEmit -= 1.0f;
		}
	}

	while(toEmit >= 1.0f)
	{
		Emit();
		toEmit -= 1.0f;
	}

	for(auto& burst : bursts)
	{
		burst.last += delta;
		if(burst.last >= burst.time)
		{
			Emit(burst.particles);
			burst.last = 0;
		}
	}

}

void ParticleSystem::OnDestroy()
{
}

void ParticleSystem::Load(Properties *prop)
{
	Component::Load(prop);

	/*
	prop->Get("lifeTime", lifetime);
	prop->Get("color", color);
	prop->Get("size", size);
	prop->Get("velocity", velocity);

	prop->Get("radius", radius);
	prop->Get("timeScale", timeScale);
	prop->Get("rate", rate);
	prop->Get("maxParticles", maxParticles);
	prop->Get("dimensions", dimensions);
	prop->Get("gravity", gravity);
	prop->Get("pausable", pausable);
	prop->Get("paused", paused);
	prop->Get("looping", looping);

	material->LoadFile(prop->Get("material"));
	*/
}

void ParticleSystem::Save(Properties *prop)
{
	Component::Save(prop);

	/*
	prop->Set("lifeTime", lifetime);
	prop->Set("color", color);
	prop->Set("size", size);
	prop->Set("velocity", velocity);

	prop->Set("radius", radius);
	prop->Set("timeScale", timeScale);
	prop->Set("rate", rate);
	prop->Set("maxParticles", maxParticles);
	prop->Set("dimensions", dimensions);
	prop->Set("gravity", gravity);
	prop->Set("pausable", pausable);
	prop->Set("paused", paused);
	prop->Set("looping", looping);
	//	prop->Set("material", material->path);
	*/
}

void ParticleSystem::Emit(uint32_t amount)
{
	for(uint32_t i = 0; i < amount; ++i)
	{
		Emit(new Particle);
	}
}

void ParticleSystem::Emit(Particle* pt, bool old)
{
	pt->system = this;
	pt->SetSizeRange(random::Range(size.start.min, size.start.max), random::Range(size.end.min, size.end.max));
	pt->SetGravity(gravity);
	pt->color.start = random::Range(color.start.min, color.start.max);
	pt->color.end = random::Range(color.end.min, color.end.max);
	pt->StartLife(random::Range(lifetime.start, lifetime.end));
	pt->Activate(true);
	GenerateVelocity(pt);
	pt->Init();
	if(!old)
	{
		particles.push_back(ptr<Particle>(pt));
	}
}

void ParticleSystem::Recycle(Particle* pt)
{
}

void ParticleSystem::GenerateVelocity(Particle* pt)
{
	float speed = random::Range(this->speed.start.min, this->speed.start.max);
	vec3 dir;
	vec3 pos;

	// Cone emitter
	pt->transform->SetPosition( pos = vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius)) );
	dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(0.0f, 1.0f), random::Range(-angle / 90, angle / 90));
	while(dir == vec3(0.0f))
	{
		// This is to prevent ever getting a 0 direction and generating an empty velocity.
		dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(0.0f, 1.0f), random::Range(-angle / 90, angle / 90));
	}
	pt->velocity = dir * speed;

	// Sphere emitter
	pt->transform->SetPosition(vec3(0));
	dir = vec3(random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f));
	pt->transform->Translate(dir * random::Range(0.0f, radius));
	pt->velocity = dir * speed;

	// Straight direction emitter
	// Cube volume, radius being half the side length
	pt->transform->SetPosition(vec3(random::Range(-radius, radius), random::Range(-radius, radius), random::Range(-radius, radius)));
	// Square area
	pt->transform->SetPosition(vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius)));
	// Point emitter
	pt->transform->SetPosition(vec3(0));
	dir = vec3(0, 1, 0);
	pt->velocity = dir * speed;

	// Beginning and ending velocity range, for an interpolating velocity.
	pt->velRange.start = random::Range(velocity.start.min, velocity.start.max);
	pt->velRange.end = random::Range(velocity.end.min, velocity.end.max);

	//return pt->velocity;
}

uint32_t ParticleSystem::GetCount()
{
	return particles.size();
}

}
