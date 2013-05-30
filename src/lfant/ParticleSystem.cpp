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

// Internal
#include <lfant/Particle.h>
#include <lfant/Random.h>
#include <lfant/Console.h>
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>

// External
#include <GL/glew.h>

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

	for(uint i = 0; i < particles.size(); ++i)
	{
		if(particles[i].active)
		{
			particles[i].Update(delta);
		}
		else if(toEmit >= 1.0f)
		{
			Emit(&particles[i]);
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
		Emit();
	}
}

void ParticleSystem::Emit(Particle* pt)
{
	if(!pt)
	{
		particles.data.emplace_back();
		pt = &particles[particles.size()-1];
	}

	pt->system = this;
	pt->SetSizeRange(random::Range(size.start.min, size.start.max), random::Range(size.end.min, size.end.max));
	pt->SetGravity(gravity);
	pt->color.start = random::Range(color.start.min, color.start.max);
	pt->color.end = random::Range(color.end.min, color.end.max);
	pt->StartLife(random::Range(lifetime.start, lifetime.end));
	pt->Activate(true);
	GenerateVelocity(pt);
	pt->Init();
}

void ParticleSystem::Recycle(Particle* pt)
{
}

void ParticleSystem::GenerateVelocity(Particle* pt)
{
	float speed = random::Range(this->speed.start.min, this->speed.start.max);
	vec3 dir;
	vec3 pos;

	switch(emitterType)
	{
	case EmitterType::Cone:
	{
		// Cone emitter
		pt->transform->SetPosition( pos = vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius)) );
		dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(0.0f, 1.0f), random::Range(-angle / 90, angle / 90));
		while(dir == vec3(0.0f))
		{
			// This is to prevent ever getting a 0 direction and generating an empty velocity.
			dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(0.0f, 1.0f), random::Range(-angle / 90, angle / 90));
		}
		break;
	}
	case EmitterType::Sphere:
	{
		// Sphere emitter
		pt->transform->SetPosition(vec3(0));
		dir = vec3(random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f));
		pt->transform->Translate(dir * random::Range(0.0f, radius));
		break;
	}
	case EmitterType::Box:
	{
		// Straight direction emitter
		// Cube volume, radius being half the side length
		pt->transform->SetPosition(vec3(random::Range(-radius, radius), random::Range(-radius, radius), random::Range(-radius, radius)));
		dir = vec3(0, 1, 0);
		break;
	}
	case EmitterType::Point:
	{
		// Point emitter
		pt->transform->SetPosition(vec3(0));
		dir = vec3(0, 1, 0);
		break;
	}
	}

	// Square area
//	pt->transform->SetPosition(vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius)));

	pt->velocity = dir * speed;

	// Beginning and ending velocity range, for an interpolating velocity.
	pt->velRange.start = random::Range(velocity.start.min, velocity.start.max);
	pt->velRange.end = random::Range(velocity.end.min, velocity.end.max);
}

uint32_t ParticleSystem::GetCount()
{
	return particles.size();
}


void ParticleSystem::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	if(material->shader->GetId() == 0)
	{
		material->shader->LoadFile();
	}

	if(material->texture->GetId() == 0)
	{
		Log("Manually loading texture.");
		material->texture->LoadFile();
	}

	if(material->shader->GetId() != 0)
	{
		Log("Adding uniforms..");
		material->shader->AddUniform("MVP");
		material->shader->AddUniform("viewportWidth");
		material->shader->AddUniform("cameraPosition");
		material->shader->AddUniform("textureSampler");
	}

	Mesh::CreateBuffer(vertexBuffer, GL_ARRAY_BUFFER);
	Mesh::CreateBuffer(normalBuffer, GL_ARRAY_BUFFER);
	Mesh::CreateBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

	loaded = true;
}

void ParticleSystem::Render()
{
	if(!material->shader->GetId() || !material->texture->GetId())
	{
		return;
	}

	glBindVertexArray(vertexArray);

	glUseProgram(material->shader->GetId());

	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView() * owner->transform->GetMatrix();
	glUniformMatrix4fv(material->shader->GetUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material->texture->GetId());
	glUniform1i(material->shader->GetUniform("textureSampler"), 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);

	glUseProgram(0);
}

void ParticleSystem::EndRender()
{
	vertexBuffer.Destroy();
//	uvBuffer.Destroy();
	normalBuffer.Destroy();
	indexBuffer.Destroy();
	material->texture->Destroy();
	glDeleteVertexArrays(1, &vertexArray);
	loaded = false;
}

}
