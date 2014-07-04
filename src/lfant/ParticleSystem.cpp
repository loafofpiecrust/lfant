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
#include <lfant/Thread.h>
#include <lfant/Input.h>
#include <lfant/Window.h>

// External
#include <GL/glew.h>

namespace lfant {

IMPLEMENT_TYPE(Component, ParticleSystem)

ParticleSystem::ParticleSystem()
{
	render = true;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init()
{
	BeginRender();
}

void ParticleSystem::Update()
{
	if(paused)
	{
		return;
	}

	if(rate > 0.0f)
	{
		toEmit += rate * GetGame()->time->deltaTime;
	//	Log("ParticleSystem updating, toEmit = ", toEmit);
		if(toEmit >= 1.0f)
		{
			Emit(floor(toEmit));
			toEmit -= floor(toEmit);
		}
	}

	for(uint i = 0; i < particles.size(); ++i)
	{
		if(!&particles[i])
		{
			particles.erase(particles.begin()+i);
			--i;
			continue;
		}
		if(particles[i].active)
		{
			particles[i].Update(GetGame()->time->deltaTime);

			if(particles[i].lifetime <= 0.0f)
			{
				Recycle(&particles[i]);
			}

		//	vertexBuffer[i] = particles[i]->position;
		//	colorBuffer[i] = particles[i]->GetColor();
		//	sizeBuffer[i] = particles[i]->GetSize();
			particleBuffer[i].position = particles[i].position;
			particleBuffer[i].color = (u8vec4)round(particles[i].GetColor()*255.0f);
			particleBuffer[i].size = particles[i].GetSize();

			if(gravity != vec3(0))
			{
				particles[i].ApplyForce(gravity, GetGame()->time->deltaTime);
			}
		}
	}

	for(uint i = 0; i < bursts.size(); ++i)
	{
		if(bursts[i].current <= 0.0f)
		{
			Emit(bursts[i].particles);
			bursts[i].current = bursts[i].time;
		}
		bursts[i].current -= GetGame()->time->deltaTime;
	}

	if(GetGame()->input->GetButtonDown("ShowFPS"))
	{
		GetGame()->Log("Current particle count: ", GetCount());
	}

}

void ParticleSystem::PostUpdate()
{
	Render();
}

void ParticleSystem::OnDestroy()
{
	EndRender();
}

void ParticleSystem::UpdatePosition(Particle* pt)
{
	for(uint i = 0; i < particles.size(); ++i)
	{
	//	if(particles[i] == pt)
		{
		//	vertexBuffer[i] = pt->position;
		//	colorBuffer[i] = pt->color.start;
		//	sizeBuffer[i] = pt->GetSize();
		}
	}
}

void ParticleSystem::Serialize(Properties *prop)
{
	Component::Serialize(prop);

	prop->ValueArray<Burst>("burst", bursts, [](Burst& burst,Properties* prop)
	{
		prop->Value("time", &burst.time);
		prop->Value("particles", &burst.particles);
		burst.current = burst.time;
	});

	prop->Value("lifetime", &lifetime);
	prop->Value("color", &color);
	prop->Value("size", &size);
	prop->Value("velocity", &velocity);

//	thread::Sleep(1500);

	prop->Value("radius", &radius);
	prop->Value("angle", &angle);
//	prop->Value("timeScale", timeScale);
	prop->Value("rate", &rate);
	prop->Value("maxParticles", &maxParticles);
	prop->Value("dimensions", &dimensions);
	prop->Value("gravity", &gravity);
	prop->Value("pausable", &pausable);
	prop->Value("paused", &paused);
	prop->Value("looping", &looping);
	prop->Value("inheritTransform", &inheritTransform);
//	prop->Value("type", &emitterType);

//	emitterType = (EmitterType)prop->Get<byte>("type");

//	material.LoadFile(prop->Value("material"));
}

void ParticleSystem::Emit(uint32 amount)
{
//	Log("Emitting ", amount, " particles");
	for(uint32 i = 0; i < amount; ++i)
	{
		Emit();
	}
}

void ParticleSystem::Emit(Particle* pt)
{
	if(!pt)
	{
		if(recycle.size() > 0)
		{
		//	Log("Recycling old particle");
			pt = recycle[0];
			recycle.pop_front();
		}
		else
		{
			if(GetCount() >= maxParticles)
			{
				return;
			}
		//	Log("Creating new particle");
		//	particles.push_back(pt = new Particle);
			particles.emplace_back();
			pt = &particles[particles.size()-1];
		//	vertexBuffer.push_back(vec3(0));
		//	colorBuffer.push_back(vec4(1));
		//	sizeBuffer.push_back(1);
			particleBuffer.push_back({vec3(0), vec4(1), 1.0f});
			rewriteBuffer = true;
		}
	}

//	pt->system = this;
	pt->size = random::Range(size.start.min, size.start.max);
//	pt->SetGravity(gravity);
	pt->color = random::Range(color.start.min, color.start.max);
	pt->lifetime = random::Range(lifetime.start, lifetime.end);
	pt->Activate(true);
	GenerateVelocity(pt);
	pt->Init();
}

void ParticleSystem::Recycle(Particle* pt)
{
	/*
	if(recycle.size() > glm::ceil(maxParticles/10))
	{
		delete pt;
		vertexBuffer.pop_back();
		colorBuffer.pop_back();
		sizeBuffer.pop_back();
		return;
	}
	*/
	recycle.push_back(pt);
	pt->active = false;
	pt->position = vec3(0);
}

void ParticleSystem::GenerateVelocity(Particle* pt)
{
//	float speed = random::Range(this->speed.start.min, this->speed.start.max);
	vec3 dir;
	vec3 pos;

	switch(emitterType)
	{
	case EmitterType::Cone:
	{
		// Cone emitter
	//	pos = vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius));
		pos = vec3(0);
		float ad = angle/180.0f;
		dir = owner->transform->GetDirection() + vec3(random::Range(-ad, ad), 1.0f, random::Range(-ad, ad));
		break;
	}
	case EmitterType::Sphere:
	{
		// Sphere emitter
		dir = vec3(random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f));
		pos = dir * random::Range(0.0f, radius);
		break;
	}
	case EmitterType::Box:
	{
		// Straight direction emitter
		// Cube volume, radius being half the side length
		pos = vec3(random::Range(-radius, radius), random::Range(-radius, radius), random::Range(-radius, radius));
		dir = owner->transform->GetDirection();
		break;
	}
	case EmitterType::Point:
	{
		// Point emitter
		pos = vec3(0);
		dir = owner->transform->GetDirection();
		break;
	}
	}

	// Square area
//	pt->transform->SetPosition(vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius)));

	if(!inheritTransform)
	{
		pos += owner->transform->GetWorldPosition();
	}
//	Log("Particle position: ", lexical_cast<string>(pos));
	pt->SetPosition(pos);
//	pt->velocity = dir * speed;

	// Beginning and ending velocity range, for an interpolating velocity.
//	pt->velRange.start = random::Range(velocity.start.min, velocity.start.max);
//	pt->velRange.end = random::Range(velocity.end.min, velocity.end.max);

	vec3 vrandstart = random::Range(velocity.start.min, velocity.start.max);
	vec3 vrandend = random::Range(velocity.end.min, velocity.end.max);

	pt->velocity = dir * vrandstart;
	pt->SetParamDiffs(dir * vrandend, random::Range(color.end.min, color.end.max), random::Range(size.end.min, size.end.max));
}

uint32 ParticleSystem::GetCount()
{
	return particles.size();
}


void ParticleSystem::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}

	material.texture = Texture::LoadFile("textures/Particle.png");

//	glGenVertexArrays(1, &vertexArray);
//	glBindVertexArray(vertexArray);

//	if(!material.shader->GetId())
	{
		material.shader = Shader::LoadFile("shaders/particles/Diffuse.vert", "shaders/particles/Diffuse.frag", "shaders/particles/Diffuse.geom");
	}

//	if(!material.texture->GetId())
	{
//		material.texture->LoadFile();
	}

	if(material.shader->GetId())
	{
		material.shader->AddUniform("model");
		material.shader->AddUniform("view");
		material.shader->AddUniform("projection");
		material.shader->AddUniform("viewportWidth");
		material.shader->AddUniform("cameraPosition");
		material.shader->AddUniform("systemPosition");
		material.shader->AddUniform("textureSampler");
	}

//	CreateBuffer(vertexBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
//	CreateBuffer(colorBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
//	CreateBuffer(sizeBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	Geometry::CreateBuffer(particleBuffer, GL_ARRAY_BUFFER, GL_STREAM_DRAW);
//	CreateBuffer(particles, GL_ARRAY_BUFFER, GL_STREAM_DRAW);

	loaded = true;
}

void ParticleSystem::Render()
{
	if(!material.shader->GetId() || !material.texture->GetId())
	{
		return;
	}
//	glDisable(GL_DEPTH_TEST);

//	glEnable(GL_POINT_SPRITE);
//	glEnable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_GEOMETRY_PROGRAM_POINT_SIZE);

//	glBindVertexArray(vertexArray);

	glUseProgram(material.shader->GetId());

	material.shader->SetUniform("model", owner->transform->GetMatrix());


	material.shader->SetUniform("projection", GetGame()->scene->mainCamera->GetProjection());
	material.shader->SetUniform("view", GetGame()->scene->mainCamera->GetView());
	material.shader->SetUniform("cameraPosition", GetGame()->scene->mainCamera->owner->transform->GetRelativeWorldPosition());
	material.shader->SetUniform("viewportWidth", GetGame()->window->GetSize().x);

	if(inheritTransform)
	{
		material.shader->SetUniform("systemPosition", owner->transform->GetRelativeWorldPosition());
	}
	else
	{
		material.shader->SetUniform("systemPosition", vec3(0));
	}

	material.texture->Bind();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	material.shader->SetUniform("textureSampler", material.texture.get());

	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer.id);
	if(rewriteBuffer)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertex) * particleBuffer.size(), &particleBuffer[0], GL_STREAM_DRAW);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), &particles[0], GL_STREAM_DRAW);
		rewriteBuffer = false;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ParticleVertex) * particleBuffer.size(), &particleBuffer[0]);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * particles.size(), &particles[0]);
	}

	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertexBuffer.size(), &vertexBuffer[0], GL_DYNAMIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, position));
//	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Particle), (void*)offsetof(Particle, position));

	glEnableVertexAttribArray(1);
//	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * colorBuffer.size(), &colorBuffer[0], GL_DYNAMIC_DRAW);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, color));
//	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Particle), (void*)offsetof(Particle, color));

	glEnableVertexAttribArray(2);
//	glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeBuffer.size(), &sizeBuffer[0], GL_DYNAMIC_DRAW);
//	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, size));
//	glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(Particle), (void*)offsetof(Particle, size));

	glDrawArrays(GL_POINTS, 0, particleBuffer.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);

	material.texture->Unbind();

	glBindVertexArray(0);

	glUseProgram(0);

//	glDisable(GL_POINT_SPRITE);
//	glDisable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_DEPTH_TEST);
}

void ParticleSystem::EndRender()
{
//	vertexBuffer.Destroy();
//	uvBuffer.Destroy();
//	normalBuffer.Destroy();
//	indexBuffer.Destroy();
	material.texture->Destroy();
//	glDeleteVertexArrays(1, &vertexArray);
	particleBuffer.Destroy();
	loaded = false;
}

}
