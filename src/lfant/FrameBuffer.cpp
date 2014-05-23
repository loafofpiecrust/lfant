/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-06-12 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/FrameBuffer.h>

// Internal
#include <lfant/Console.h>
#include <lfant/Thread.h>
#include <lfant/Renderer.h>
#include <lfant/Camera.h>
#include <lfant/Scene.h>
#include <lfant/Transform.h>
#include <lfant/Input.h>
#include "lfant/Window.h"

// External
#if !ANDROID
#include <GL/glew.h>
#else
//#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

namespace lfant {

FrameBuffer* FrameBuffer::current = nullptr;

FrameBuffer::FrameBuffer(Game* game) :
	game(game)
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Clear()
{
	GetGame()->Log("Clearing framebuffer");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Init()
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	GetGame()->Log(glGetError());

	drawBuffers.clear();
	Texture* tex = nullptr;
	if(textures.size() > 0)
	{
		for(uint i = 0; i < textures.size(); ++i)
		{
			tex = textures[i];
		//	tex->scaleFilter = Texture::ScaleFilter::Linear;
			tex->wrapMode = Texture::WrapMode::Clamp;
		//	tex->mode = GL_TEXTURE_2D_MULTISAMPLE;
			tex->size = uvec2(rect.width, rect.height);
			tex->SetIndex(startIndex+i);
			tex->InitData(0);

			uint32 attachment = startIndex+GL_COLOR_ATTACHMENT0+i;
			GetGame()->Log("Setting up textures for FrameBuffer, tex ", i, " gets attachment ", attachment, ". GL_COLOR_ATTACHMENT0 = ", GL_COLOR_ATTACHMENT0);

			tex->Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex->GetMode(), tex->GetId(), 0);

			drawBuffers.push_back(attachment);
		}
	}

	if(hasDepth)
	{
	/*	glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		GetGame()->Log("stuff lel ", depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, rect.width, rect.height);
		GetGame()->Log(glGetError());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	*/
		if(depthTexture)
		{
		//	depthTexture->SetFormat(Texture::Format::Depth32F_Stencil8, Texture::Format::Depth);
			depthTexture->SetFormat(Texture::Format::Depth32F, Texture::Format::Depth);
			depthTexture->wrapMode = Texture::WrapMode::Clamp;
			depthTexture->dataType = Texture::DataType::Float;
			depthTexture->size = uvec2(rect.width, rect.height);
			depthTexture->SetIndex(startIndex+textures.size());
			depthTexture->InitData(0);

			depthTexture->Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthTexture->GetMode(), depthTexture->GetId(), 0);

			GetGame()->Log(glGetError());
		}
	}

	DrawBuffers();

	BeginRender();

	Unbind();
}

void FrameBuffer::Deinit()
{
	posBuffer.Destroy();
	shader.reset();

	for(auto& tex : textures)
	{
		tex->Destroy();
	}
	textures.clear();

	depthTexture->Destroy();
	depthTexture.reset();

	glDeleteFramebuffers(1, &id);
}

Game* FrameBuffer::GetGame() const
{
	return game;
}

void FrameBuffer::AddTexture(string name, Texture::Format form1, Texture::Format form2)
{
	Texture* tex = new Texture;
	tex->path = name;
	tex->SetFormat(form1, form2);
	textures.push_back(tex);
}

void FrameBuffer::AddDepthTexture(string name, Texture::Format form1, Texture::Format form2)
{
	if(!depthTexture)
	{
		depthTexture = new Texture;
	}
	depthTexture->path = name;
	hasDepth = true;
}

void FrameBuffer::GetTextures(Shader* sh) const
{
	if(textures.size() > 0)
	{
		sh->Bind();
		for(uint i = 0; i < textures.size(); ++i)
		{
			textures[i]->Bind();
			sh->SetUniform(textures[i]->path, textures[i]);
		}
		textures[0]->Unbind();
		sh->Unbind();
	}
}

Texture* FrameBuffer::GetTexture(string name) const
{
	if(name == depthTexture->path)
	{
		return depthTexture;
	}
	for(uint i = 0; i < textures.size(); ++i)
	{
		if(textures[i]->path == name)
		{
			return textures[i];
		}
	}
	return nullptr;
}

void FrameBuffer::BindTextures(Shader* sh)
{
	if(!sh)
	{
		for(uint i = 0; i < textures.size(); ++i)
		{
			textures[i]->Bind();
		}
	}
	else
	{
		for(uint i = 0; i < textures.size(); ++i)
		{
			sh->SetUniform(textures[i]->path, textures[i]);
		}
	}
}

FrameBuffer* FrameBuffer::GetCurrent()
{
//	GetGame()->Log("Getting current fbo, ", FrameBuffer::current);
	return FrameBuffer::current;
}

void FrameBuffer::Bind()
{
//	GetGame()->Log("Binding FBO ", id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	FrameBuffer::current = this;
}

void FrameBuffer::Unbind()
{
//	GetGame()->Log("Unbinding FBO");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	FrameBuffer::current = nullptr;
}

void FrameBuffer::BindRead()
{
#if !LFANT_GLES
	glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, id);
#endif
	FrameBuffer::current = this;
}

void FrameBuffer::UnbindRead()
{
#if !LFANT_GLES
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
	FrameBuffer::current = nullptr;
}

void FrameBuffer::DrawBuffers()
{
	glDrawBuffer(GL_NONE);
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
#if !LFANT_GLES
	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
#endif
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		GetGame()->Log("Framebuffer failed to draw.");
	}
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBuffer::DrawBuffer(int idx)
{
	glDrawBuffer(drawBuffers[idx]);
}

uint32 FrameBuffer::GetAttachment(uint32 idx)
{
	return GL_COLOR_ATTACHMENT0 + idx;
}

void FrameBuffer::ResizeViewport()
{
	glViewport(rect.x, rect.y, rect.width, rect.height);
}

void FrameBuffer::Resize()
{

}

void FrameBuffer::BeginRender()
{
//	GLuint VertexArrayID;
//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);

	GetGame()->Log("FrameBuffer::BeginRender() {");
//	shader = new Shader;
	GetGame()->Log("Shader::LoadFile()");
//	shader->LoadFile("shaders/FrameBuffer.vert", "shaders/FrameBuffer.frag");
	shader = Shader::LoadFile("shaders/FrameBuffer.vert", "shaders/FrameBuffer.frag");
	GetGame()->Log(glGetError());

	for(uint i = 0; i < textures.size(); ++i)
	{
		shader->AddUniform(textures[i]->path);
	}
	if(!depthTexture->path.empty())
	{
		shader->AddUniform(depthTexture->path);
	}
	GetGame()->Log(glGetError());

	GetGame()->Log("Add uniforms to shader");
	shader->AddUniform("cameraRange");
	shader->AddUniform("focalDepth");
	shader->AddUniform("focalLength");
	shader->AddUniform("fstop");
	shader->AddUniform("focus");
	shader->AddUniform("textureSize");
	shader->AddUniform("useDof");


	GetGame()->Log("Make buffers");

	posBuffer.push_back(vec2(-1, -1));
	posBuffer.push_back(vec2(1, -1));
	posBuffer.push_back(vec2(-1, 1));

	posBuffer.push_back(vec2(-1, 1));
	posBuffer.push_back(vec2(1, -1));
	posBuffer.push_back(vec2(1, 1));

	Geometry::CreateBuffer(posBuffer, GL_ARRAY_BUFFER);
	GetGame()->Log("}");

	GetGame()->Log(glGetError());
}



void FrameBuffer::Render()
{
	shader->Bind();

//	GetGame()->Log(glGetError());

//	shader->SetUniform("cameraPosition", game->scene->mainCamera->owner->transform->GetWorldPosition());
//	shader->SetUniform("cameraDof", vec2(game->scene->mainCamera->dof - game->scene->mainCamera->dofWidth, game->scene->mainCamera->dof + game->scene->mainCamera->dofWidth));
	if(GetGame()->scene->mainCamera)
	{
		shader->SetUniform("focalDepth", GetGame()->scene->mainCamera->focalDepth);
		shader->SetUniform("focalLength", GetGame()->scene->mainCamera->focalLength);
		shader->SetUniform("fstop", GetGame()->scene->mainCamera->aperture);
		shader->SetUniform("focus", GetGame()->scene->mainCamera->focus);
		shader->SetUniform("cameraRange", vec2_cast<vec2>(GetGame()->scene->mainCamera->GetViewRange()));
		shader->SetUniform("useDof", GetGame()->scene->mainCamera->useDof);
	}
	shader->SetUniform("textureSize", (vec2)GetGame()->window->GetSize());
//	GetGame()->Log(glGetError());

	for(uint32 i = 0; i < textures.size(); ++i)
	{
		if(shader->GetUniform(textures[i]->path) != -1)
		{
		//	shader->SetUniform(textures[i]->path, textures[i]->GetIndex());
			shader->SetUniform(textures[i]->path, textures[i]);
		}
	}
	if(depthTexture) shader->SetUniform(depthTexture->path, depthTexture);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer.id);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, posBuffer.size());

//	GetGame()->Log(glGetError());

//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);


/*	for(uint i = 0; i < textures.size(); ++i)
	{
		textures[i]->Unbind();
	}

	shader->Unbind();*/
}

void FrameBuffer::EndRender()
{
}

}
