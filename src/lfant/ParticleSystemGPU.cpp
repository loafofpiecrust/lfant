/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-08-02 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#include <GL/glew.h>

#define BOOST_COMPUTE_HAVE_GL
#include <lfant/ParticleSystemGPU.h>

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
#include <boost/compute/interop/opengl.hpp>

namespace compute = boost::compute;

namespace lfant {

IMPLEMENT_TYPE(Component, ParticleSystemGPU)

ParticleSystemGPU::ParticleSystemGPU()
{
}

ParticleSystemGPU::~ParticleSystemGPU()
{
}

void ParticleSystemGPU::Init()
{
	BeginRender();
//	updateKernel->Get()->set_arg(9, gravity);
}

void ParticleSystemGPU::Update()
{
	if(paused)
	{
		return;
	}

	if(rate > 0.0f && GetCount() < maxParticles)
	{
		toEmit += rate * GetGame()->time->deltaTime;
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
		bursts[i].current -= GetGame()->time->deltaTime;
	}

	if(GetGame()->input->GetButtonDown("ShowFPS"))
	{
		GetGame()->Log("Current particle count: ", GetCount());
		vec4 pos[10];
		glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, 10*sizeof(vec4), &pos);
		for(int i = 0; i < 10; ++i)
		{
			std::cout << "value "<<i<<": "<<lexical_cast<string>(pos[i])<<";\n";
		}
	}

}

void ParticleSystemGPU::PostUpdate()
{
	Render();
}

void ParticleSystemGPU::Deinit()
{
	EndRender();
}

void ParticleSystemGPU::UpdatePosition(Particle* pt)
{
}

void ParticleSystemGPU::Serialize(Properties *prop)
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

	GetGame()->Log("Loaded velocity: ", lexical_cast<string>(velocity));
	GetGame()->Log("Loaded lifetime: ", lexical_cast<string>(lifetime));
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

	material.LoadFile(prop->GetString("material"));
}

void ParticleSystemGPU::Recycle(Particle* pt)
{
	recycle.push_back(pt);
}

void ParticleSystemGPU::Emit(uint32 count)
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
		particle.vel = vec4(dir * random::Range(velocity.start.min, velocity.start.max), 0.0f);
		particle.size = random::Range(size.start.min, size.start.max);
		particle.color = random::Range(color.start.min, color.start.max);
		particle.life = random::Range(lifetime.start, lifetime.end);

		particle.velChange = (vec4(dir * random::Range(velocity.end.min, velocity.end.max), 0.0f) - particle.vel) / particle.life;
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

		compute::buffer pb(GetGame()->openCL->context, sizeof(Particle), CL_MEM_READ_ONLY);
		GetGame()->openCL->queue.enqueue_write_buffer(pb, &particle);
		GetGame()->openCL->queue.finish();
		emitKernel->Get()->set_arg(9, pb);
		emitKernel->Get()->set_arg(10, GetCount()+1);

		{
			boost::compute::opengl_enqueue_acquire_buffer(glbuffers[0], GetGame()->openCL->queue);
			boost::compute::opengl_enqueue_acquire_buffer(glbuffers[1], GetGame()->openCL->queue);
			boost::compute::opengl_enqueue_acquire_buffer(glbuffers[2], GetGame()->openCL->queue);

		//	GetGame()->openCL->queue.enqueue_acquire_gl_objects(3, &clbuffers[0].get());
		//	GetGame()->openCL->queue.finish();

			GetGame()->openCL->queue.enqueue_1d_range_kernel(*emitKernel->Get(), 0, 1);
		//	GetGame()->openCL->queue.finish();

			boost::compute::opengl_enqueue_release_buffer(glbuffers[0], GetGame()->openCL->queue);
			boost::compute::opengl_enqueue_release_buffer(glbuffers[1], GetGame()->openCL->queue);
			boost::compute::opengl_enqueue_release_buffer(glbuffers[2], GetGame()->openCL->queue);
		//	GetGame()->openCL->queue.enqueue_release_gl_objects(3, &clbuffers[0].get());
			GetGame()->openCL->queue.finish();
		}

		++particleCount;
		GetGame()->Log("Added particle to system, count: ", GetCount());
	}


}

uint32 ParticleSystemGPU::GetCount()
{
	return particleCount;
}


void ParticleSystemGPU::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}

	material.texture = Texture::LoadFile("textures/Particle.png");

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	if(!material.shader)
	{
		material.shader = Shader::LoadFile("shaders/particles/Diffuse.vert", "shaders/particles/Diffuse.frag", "shaders/particles/Diffuse.geom");
	}

//	if(!material.texture->GetId())
	{
//		GetGame()->Log("Manually loading texture.");
//		material.texture->LoadFile();
	}

	if(material.shader)
	{
		GetGame()->Log("Adding uniforms..");
		material.shader->AddUniform("mvp");
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
//	CreateBuffer(particleBuffer, GL_ARRAY_BUFFER, GL_STREAM_DRAW);
//	CreateBuffer(particles, GL_ARRAY_BUFFER, GL_STREAM_DRAW);

	GetGame()->Log("Loading kernels");
	updateKernel = GetGame()->openCL->LoadFile("kernels/particleSystem.cl", "Update");
	emitKernel = GetGame()->openCL->LoadFile("kernels/particleSystem.cl", "Emit");

	GetGame()->Log("Making buffers");

	std::vector<vec4> initData(maxParticles, vec4(0,0,0,1));

	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles*sizeof(vec4), &initData[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles*sizeof(vec4), &initData[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &sizeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles*sizeof(float), &initData[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glFinish();


	GetGame()->Log("Crossing gl buffers to cl");
	glbuffers.clear();
//	clbuffers.resize(8);
	glbuffers.emplace_back(GetGame()->openCL->context, posBuffer);
	glbuffers.emplace_back(GetGame()->openCL->context, colorBuffer);
	glbuffers.emplace_back(GetGame()->openCL->context, sizeBuffer);

	GetGame()->Log("Adding cl-only buffers");
	clbuffers.clear();
	clbuffers.emplace_back(GetGame()->openCL->context, maxParticles*sizeof(vec4));
	clbuffers.emplace_back(GetGame()->openCL->context, maxParticles*sizeof(vec4));
	clbuffers.emplace_back(GetGame()->openCL->context, maxParticles*sizeof(vec4));
	clbuffers.emplace_back(GetGame()->openCL->context, maxParticles*sizeof(float));
	clbuffers.emplace_back(GetGame()->openCL->context, maxParticles*sizeof(float));
//	clbuffers.resize(7);

	GetGame()->Log("Setting up gl args in kernel");
	uint arg = 0;
	for(int i = 0; i < glbuffers.size(); ++i)
	{
		emitKernel->Get()->set_arg(arg, glbuffers[i]);
		updateKernel->Get()->set_arg(arg, glbuffers[i]);
		++arg;
	}
	GetGame()->Log("cl args from ", arg);
	for(uint i = 0; i < clbuffers.size(); ++i)
	{
		emitKernel->Get()->set_arg(arg, clbuffers[i]);
		updateKernel->Get()->set_arg(arg, clbuffers[i]);
		++arg;
	}

	loaded = true;
}

//static uint renderSleep = 150;

void ParticleSystemGPU::Render()
{
	if(GetCount() <= 0)
	{
		return;
	}
	if(!material.shader->GetId() || !material.texture->GetId())
	{
		return;
	}
//	GetGame()->Log("Rendering ParticleSystem");
//	thread::Sleep(renderSleep);
//	glDisable(GL_DEPTH_TEST);

//	glEnable(GL_POINT_SPRITE);
//	glEnable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_GEOMETRY_PROGRAM_POINT_SIZE);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glFinish();
	{
		boost::compute::opengl_enqueue_acquire_buffer(glbuffers[0], GetGame()->openCL->queue);
		boost::compute::opengl_enqueue_acquire_buffer(glbuffers[1], GetGame()->openCL->queue);
		boost::compute::opengl_enqueue_acquire_buffer(glbuffers[2], GetGame()->openCL->queue);

		uint arg = 0;
		for(int i = 0; i < glbuffers.size(); ++i)
		{
			emitKernel->Get()->set_arg(arg, glbuffers[i]);
			updateKernel->Get()->set_arg(arg, glbuffers[i]);
			++arg;
		}
		GetGame()->Log("cl args from ", arg);
		for(uint i = 0; i < clbuffers.size(); ++i)
		{
			emitKernel->Get()->set_arg(arg, clbuffers[i]);
			updateKernel->Get()->set_arg(arg, clbuffers[i]);
			++arg;
		}

	//	GetGame()->openCL->queue.enqueue_acquire_gl_objects(3, &clbuffers[0].get());
		GetGame()->openCL->queue.finish();
	//
		updateKernel->Get()->set_arg(8, GetGame()->time->deltaTime);
		GetGame()->openCL->queue.enqueue_1d_range_kernel(*updateKernel->Get(), 0, GetCount());
		GetGame()->openCL->queue.finish();

		boost::compute::opengl_enqueue_release_buffer(glbuffers[0], GetGame()->openCL->queue);
		boost::compute::opengl_enqueue_release_buffer(glbuffers[1], GetGame()->openCL->queue);
		boost::compute::opengl_enqueue_release_buffer(glbuffers[2], GetGame()->openCL->queue);
	//	GetGame()->openCL->queue.enqueue_release_gl_objects(3, &clbuffers[0].get());
		GetGame()->openCL->queue.finish();
		glFinish();
	}

//	glBindVertexArray(vertexArray);

	material.shader->Bind();

//	mat4 mvp = GetGame()->scene->mainCamera->GetProjection() * GetGame()->scene->mainCamera->GetView() /* owner->transform->GetMatrix()*/;
//	material.shader->SetUniform("mvp", mvp);
	material.shader->SetUniform("projection", GetGame()->scene->mainCamera->GetProjection());
	material.shader->SetUniform("view", GetGame()->scene->mainCamera->GetView());
	material.shader->SetUniform("cameraPosition", GetGame()->scene->mainCamera->owner->transform->GetRelativeWorldPosition());

	if(inheritTransform)
	{
		material.shader->SetUniform("systemPosition", owner->transform->GetRelativeWorldPosition());
	}
	else
	{
	//	GetGame()->Log("Rendering particle independent from system.");
		material.shader->SetUniform("systemPosition", vec3(0));
	}

	material.texture->Bind();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	material.shader->SetUniform("textureSampler", material.texture.get());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, glbuffers[0].get_opengl_object());
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
	glVertexAttribPointer(2, 1, GL_FLOAT, false, 0, (void*)0);

//	GetGame()->Log("Rendering");
//	thread::Sleep(renderSleep);
//	double t = GetGame()->time->GetTime();
	glDrawArrays(GL_POINTS, 0, GetCount());
//	GetGame()->Log("Rendering took ", GetGame()->time->GetTime() - t, " seconds");
//	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);

//	material.texture->Unbind();

//	glBindVertexArray(0);

//	glUseProgram(0);


//	glDisable(GL_POINT_SPRITE);
//	glDisable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_DEPTH_TEST);
}

void ParticleSystemGPU::EndRender()
{
	glDeleteBuffers(1, &posBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &sizeBuffer);
//	material.texture->Destroy();
	glDeleteVertexArrays(1, &vertexArray);
	loaded = false;
}

}
