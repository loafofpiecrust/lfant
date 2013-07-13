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
#include <GL/glew.h>

#define BOOST_COMPUTE_HAVE_GL
#include <lfant/ParticleSystem.h>

// Internal
//#include <lfant/Particle.h>
#include <lfant/Random.h>
#include <lfant/Console.h>
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Thread.h>
#include <lfant/Input.h>
#include <lfant/Transform.h>

// External

namespace compute = boost::compute;

namespace lfant {

IMPLEMENT_COMP(ParticleSystem)

ParticleSystem::ParticleSystem() :
	material(new Material)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init()
{
	BeginRender();
//	updateKernel->Get()->set_arg(9, gravity);
}

void ParticleSystem::Update()
{
	if(paused)
	{
		return;
	}

	if(rate > 0.0f && GetCount() < maxParticles)
	{
		toEmit += rate * game->time->deltaTime;
		if(toEmit >= 1.0f)
		{
			Emit(floor(toEmit));
			toEmit -= floor(toEmit);
		}
	}

	for(uint i = 0; i < bursts.size(); ++i)
	{
		if(bursts[i].current <= 0.0f)
		{
			Emit(bursts[i].particles);
			bursts[i].current = bursts[i].time;
		}
		bursts[i].current -= game->time->deltaTime;
	}

	if(game->input->GetButtonDown("ShowFPS"))
	{
		Log("Current particle count: ", GetCount());
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
}

void ParticleSystem::Load(Properties *prop)
{
	Component::Load(prop);

	for(auto& pb : prop->GetChildren("burst"))
	{
		bursts.emplace_front();
		Burst& burst = bursts[0];
		pb->Get("time", burst.time);
		pb->Get("particles", burst.particles);
		burst.current = burst.time;
	}

	prop->Get("lifetime", lifetime);
	prop->Get("color", color);
	prop->Get("size", size);
	prop->Get("velocity", velocity);

	Log("Loaded velocity: ", lexical_cast<string>(velocity));
//	thread::Sleep(1500);

	prop->Get("radius", radius);
	prop->Get("angle", angle);
//	prop->Get("timeScale", timeScale);
	prop->Get("rate", rate);
	prop->Get("maxParticles", maxParticles);
	prop->Get("dimensions", dimensions);
	prop->Get("gravity", gravity);
	prop->Get("pausable", pausable);
	prop->Get("paused", paused);
	prop->Get("looping", looping);
	prop->Get("inheritTransform", inheritTransform);

	emitterType = (EmitterType)prop->Get<byte>("type");

	material->LoadFile(prop->Get("material"));
}

void ParticleSystem::Save(Properties *prop)
{
	Component::Save(prop);

	/*
	prop->Set("lifetime", lifetime);
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

void ParticleSystem::Recycle(Particle* pt)
{
	recycle.push_back(pt);
}

void ParticleSystem::Emit(uint32_t count)
{
	for(uint i = 0; i < count; ++i)
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
			dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(-0.1f, 1.0f), random::Range(-angle / 90, angle / 90));
			while(dir == vec3(0.0f))
			{
				// This is to prevent ever getting a 0 direction and generating an empty velocity.
				dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(-0.1f, 1.0f), random::Range(-angle / 90, angle / 90));
			}
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
			dir = vec3(0, 1, 0);
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

		if(!inheritTransform)
		{
			pos += owner->transform->GetWorldPosition();
		}

		glFinish();
		emitKernel->Get()->set_arg(8, GetCount());

		Particle particle;

		particle.pos = vec4(pos.x, pos.y, pos.z, 1.0f);
		particle.vel = vec4(dir * random::Range(velocity.start.min, velocity.start.max), 1.0f);
		particle.size = random::Range(size.start.min, size.start.max);
		particle.color = random::Range(color.start.min, color.start.max);
		particle.life = random::Range(lifetime.start, lifetime.end);

		particle.velChange = (vec4(dir * random::Range(velocity.end.min, velocity.end.max), 1.0f) - particle.vel) / particle.life;
		particle.colorChange = (random::Range(color.end.min, color.end.max) - particle.color) / particle.life;
		particle.sizeChange = (random::Range(size.end.min, size.end.max) - particle.size) / particle.life;

		printf("\n[Cpu]\n");
		printf("Position: %s\n", lexical_cast<string>(particle.pos).c_str());
		printf("Size: %f\n", particle.size);
		printf("Velocity: %s\n", lexical_cast<string>(particle.vel).c_str());
		printf("Color: %s\n", lexical_cast<string>(particle.color).c_str());

		printf("VelChange: %s\n", lexical_cast<string>(particle.velChange).c_str());
		printf("ColorChange: %s\n", lexical_cast<string>(particle.colorChange).c_str());
		printf("SizeChange: %f\n", particle.sizeChange);

		compute::buffer pb(game->openCL->context, sizeof(Particle), CL_MEM_READ_ONLY);
		game->openCL->queue.enqueue_write_buffer(pb, &particle);
		game->openCL->queue.finish();
		emitKernel->Get()->set_arg(9, pb);
		emitKernel->Get()->set_arg(10, GetCount()+1);

		{
			game->openCL->queue.enqueue_acquire_gl_buffer(clbuffers[0]);
			game->openCL->queue.enqueue_acquire_gl_buffer(clbuffers[1]);
			game->openCL->queue.enqueue_acquire_gl_buffer(clbuffers[2]);
		//	game->openCL->queue.enqueue_acquire_gl_objects(3, &clbuffers[0].get());
		//	game->openCL->queue.finish();

			game->openCL->queue.enqueue_1d_range_kernel(*emitKernel->Get(), 0, 1);
			game->openCL->queue.finish();

			game->openCL->queue.enqueue_release_gl_buffer(clbuffers[0]);
			game->openCL->queue.enqueue_release_gl_buffer(clbuffers[1]);
			game->openCL->queue.enqueue_release_gl_buffer(clbuffers[2]);
		//	game->openCL->queue.enqueue_release_gl_objects(3, &clbuffers[0].get());
			game->openCL->queue.finish();
		}

		++particleCount;
		Log("Added particle to system, count: ", GetCount());
	}


}

uint32_t ParticleSystem::GetCount()
{
	return particleCount;
}


void ParticleSystem::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}

	material->texture->LoadFile("textures/Particle.png");

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

//	if(!material->shader->GetId())
	{
		material->shader->LoadFile("shaders/particles/Diffuse.vert", "shaders/particles/Diffuse.frag", "shaders/particles/Diffuse.geom");
	}

//	if(!material->texture->GetId())
	{
//		Log("Manually loading texture.");
//		material->texture->LoadFile();
	}

	if(material->shader->GetId())
	{
		Log("Adding uniforms..");
		material->shader->AddUniform("mvp");
		material->shader->AddUniform("view");
		material->shader->AddUniform("projection");
		material->shader->AddUniform("viewportWidth");
		material->shader->AddUniform("cameraPosition");
		material->shader->AddUniform("systemPosition");
		material->shader->AddUniform("textureSampler");
	}

//	CreateBuffer(vertexBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
//	CreateBuffer(colorBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
//	CreateBuffer(sizeBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
//	CreateBuffer(particleBuffer, GL_ARRAY_BUFFER, GL_STREAM_DRAW);
//	CreateBuffer(particles, GL_ARRAY_BUFFER, GL_STREAM_DRAW);

	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles*sizeof(vec4), 0, GL_STREAM_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles*sizeof(vec4), 0, GL_STREAM_DRAW);

	glGenBuffers(1, &sizeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles*sizeof(float), 0, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glFinish();
	updateKernel = game->openCL->LoadFile("kernels/particleSystem.cl", "Update");
	emitKernel = game->openCL->LoadFile("kernels/particleSystem.cl", "Emit");
	clbuffers.clear();
//	clbuffers.resize(8);
	clbuffers.emplace_back(compute::buffer::from_gl_buffer(game->openCL->context, posBuffer, CL_MEM_WRITE_ONLY));
	clbuffers.emplace_back(compute::buffer::from_gl_buffer(game->openCL->context, colorBuffer, CL_MEM_READ_WRITE));
	clbuffers.emplace_back(compute::buffer::from_gl_buffer(game->openCL->context, sizeBuffer, CL_MEM_READ_WRITE));

	clbuffers.emplace_back(game->openCL->context, maxParticles*sizeof(vec4));
	clbuffers.emplace_back(game->openCL->context, maxParticles*sizeof(vec4));
	clbuffers.emplace_back(game->openCL->context, maxParticles*sizeof(vec4));
	clbuffers.emplace_back(game->openCL->context, maxParticles*sizeof(float));
	clbuffers.emplace_back(game->openCL->context, maxParticles*sizeof(float));
//	clbuffers.resize(7);

	for(uint i = 0; i < clbuffers.size(); ++i)
	{
		emitKernel->Get()->set_arg(i, clbuffers[i]);
		updateKernel->Get()->set_arg(i, clbuffers[i]);
	}

	loaded = true;
}

static uint renderSleep = 150;

void ParticleSystem::Render()
{
	if(GetCount() <= 0)
	{
		return;
	}
	if(!material->shader->GetId() || !material->texture->GetId())
	{
		return;
	}
//	Log("Rendering ParticleSystem");
//	thread::Sleep(renderSleep);
//	glDisable(GL_DEPTH_TEST);

//	glEnable(GL_POINT_SPRITE);
//	glEnable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_GEOMETRY_PROGRAM_POINT_SIZE);
	
	{
		game->openCL->queue.enqueue_acquire_gl_buffer(clbuffers[0]);
		game->openCL->queue.enqueue_acquire_gl_buffer(clbuffers[1]);
		game->openCL->queue.enqueue_acquire_gl_buffer(clbuffers[2]);
	//	game->openCL->queue.enqueue_acquire_gl_objects(3, &clbuffers[0].get());
	//	game->openCL->queue.finish();
	//
		updateKernel->Get()->set_arg(8, game->time->deltaTime);
		game->openCL->queue.enqueue_1d_range_kernel(*updateKernel->Get(), 0, GetCount());
		game->openCL->queue.finish();

		game->openCL->queue.enqueue_release_gl_buffer(clbuffers[0]);
		game->openCL->queue.enqueue_release_gl_buffer(clbuffers[1]);
		game->openCL->queue.enqueue_release_gl_buffer(clbuffers[2]);
	//	game->openCL->queue.enqueue_release_gl_objects(3, &clbuffers[0].get());
		game->openCL->queue.finish();
	}

	glBindVertexArray(vertexArray);

	glUseProgram(material->shader->GetId());

//	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView() /* owner->transform->GetMatrix()*/;
//	material->shader->SetUniform("mvp", mvp);
	material->shader->SetUniform("projection", game->scene->mainCamera->GetProjection());
	material->shader->SetUniform("view", game->scene->mainCamera->GetView());
	material->shader->SetUniform("cameraPosition", game->scene->mainCamera->owner->transform->GetWorldPosition());

	if(inheritTransform)
	{
		material->shader->SetUniform("systemPosition", owner->transform->GetWorldPosition());
	}
	else
	{
	//	Log("Rendering particle independent from system.");
		material->shader->SetUniform("systemPosition", vec3(0));
	}

	material->texture->Bind();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	material->shader->SetUniform("textureSampler", material->texture);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, (void*)0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
	glVertexAttribPointer(2, 1, GL_FLOAT, false, 0, (void*)0);
	
//	Log("Rendering");
//	thread::Sleep(renderSleep);
//	double t = game->time->GetTime();
	glDrawArrays(GL_POINTS, 0, GetCount());
//	Log("Rendering took ", game->time->GetTime() - t, " seconds");
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);

	material->texture->Unbind();

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
	material->texture->Destroy();
	glDeleteVertexArrays(1, &vertexArray);
	loaded = false;
}

}
