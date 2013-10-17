/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-12 by Taylor Snead
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

#include <lfant/FrameBuffer.h>

// Internal
#include <lfant/Console.h>
#include <lfant/Thread.h>
#include <lfant/Renderer.h>
#include <lfant/Camera.h>
#include <lfant/Scene.h>
#include <lfant/Transform.h>
#include <lfant/Input.h>

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

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Clear()
{
	Log("Clearing framebuffer");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Init()
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

//	int bcount = textures.size() + (hasDepth ? 1 : 0);
	drawBuffers.clear();
//	bcount = 0;
	Texture* tex = nullptr;
	if(textures.size() > 0)
	{
	//	Texture::resetUnit(textureUnitOffset);
		for(uint i = 0; i < textures.size(); ++i)
		{
			tex = textures[i];
		//	Texture* tex = Texture::newFromNextUnit();
		//	tex->setupForFramebuffer(size.x, size.y, GL_RGB32F);
		//	if(linearFilter) tex->SetFilters(GL_LINEAR, GL_LINEAR);
			tex->scaleFilter = Texture::ScaleFilter::Linear;
			tex->wrapMode = Texture::WrapMode::Clamp;
		//	tex->mode = GL_TEXTURE_2D_MULTISAMPLE;
		//	tex->msaa = 2;
			tex->size = uvec2(rect.width, rect.height);
			tex->SetIndex(startIndex+i);
			tex->InitData(0);

			uint32_t attachment = startIndex+GL_COLOR_ATTACHMENT0+i;
			Log("Setting up textures for FrameBuffer, tex ", i, " gets attachment ", attachment, ". GL_COLOR_ATTACHMENT0 = ", GL_COLOR_ATTACHMENT0);

			tex->Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex->GetMode(), tex->GetId(), 0);
		//	glFramebufferTexture(GL_FRAMEBUFFER, attachment, tex->GetId(), 0);
		//	tex->Unbind();

			drawBuffers.push_back(attachment);
		}
	}

	if(hasDepth)
	{
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		Log("Setting up depth for FrameBuffer, size: (", rect.width, ", ", rect.height, ").");
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, rect.width, rect.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		if(depthTexture)
		{
			depthTexture->SetFormat(Texture::Format::Depth32F, Texture::Format::Depth);
			depthTexture->wrapMode = Texture::WrapMode::Clamp;
		//	depthTexture->dataType = Texture::DataType::Float;
			depthTexture->size = uvec2(rect.width, rect.height);
			tex->SetIndex(startIndex+textures.size());
			depthTexture->InitData(0);

			depthTexture->Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->GetMode(), depthTexture->GetId(), 0);

		//	drawBuffers.push_back(GL_DEPTH_ATTACHMENT);
		}
	}
	/*
	if(textures.size() > 0)
	{
		for(uint i = 0; i < textures.size(); ++i)
		{
			tex = textures[i];
		}
	}
	*/
	DrawBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Deinit()
{
	EndRender();
	for(auto& tex : textures)
	{
		tex->Destroy();
		if(tex)
		{
			delete tex;
		}
	}
	glDeleteFramebuffers(1, &id);
}

void FrameBuffer::AddTexture(string name, Texture::Format form1, Texture::Format form2)
{
	textures.push_back(new Texture);
	textureNames.push_back(name);
	textures[textures.size()-1]->SetFormat(form1, form2);
}

void FrameBuffer::AddDepthTexture(string name, Texture::Format form1, Texture::Format form2)
{
	if(!depthTexture)
	{
		depthTexture = new Texture;
	}
	depthTexName = name;
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
			sh->SetUniform(textureNames[i], textures[i]);
		}
		textures[0]->Unbind();
		sh->Unbind();
	}
}

Texture* FrameBuffer::GetTexture(string name) const
{
	if(name == depthTexName)
	{
		return depthTexture;
	}
	for(uint i = 0; i < textures.size(); ++i)
	{
		if(textureNames[i] == name)
		{
			return textures[i];
		}
	}
}

void FrameBuffer::SetCurrentTexture(string name)
{
	currentTex = GetTexture(name);
}

FrameBuffer* FrameBuffer::GetCurrent()
{
//	Log("Getting current fbo, ", FrameBuffer::current);
	return FrameBuffer::current;
}

void FrameBuffer::Bind()
{
//	Log("Binding FBO ", id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	FrameBuffer::current = this;
}

void FrameBuffer::Unbind()
{
//	Log("Unbinding FBO");
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
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
#if !LFANT_GLES
	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
#endif
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log("Framebuffer failed to draw.");
	}
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

uint32_t FrameBuffer::GetAttachment(uint32_t idx)
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
	shader = new Shader;
	shader->LoadFile("shaders/FrameBuffer.vert", "shaders/FrameBuffer.frag");

	for(uint i = 0; i < textures.size(); ++i)
	{
		shader->AddUniform(textureNames[i]);
	}
	if(depthTexName != "")
	{
		shader->AddUniform(depthTexName);
	}
	
	shader->AddUniform("cameraRange");
	shader->AddUniform("focalDepth");
	shader->AddUniform("focalLength");
	shader->AddUniform("fstop");
	shader->AddUniform("focus");
	shader->AddUniform("textureSize");

	posBuffer.push_back(vec2(-1, -1));
	posBuffer.push_back(vec2(1, -1));
	posBuffer.push_back(vec2(-1, 1));

	posBuffer.push_back(vec2(-1, 1));
	posBuffer.push_back(vec2(1, -1));
	posBuffer.push_back(vec2(1, 1));

	Mesh::CreateBuffer(posBuffer, GL_ARRAY_BUFFER);
}

void FrameBuffer::Render()
{
	shader->Bind();

	for(uint i = 0; i < textures.size(); ++i)
	{
		shader->SetUniform(textureNames[i], textures[i]);
	}
	if(depthTexture)
	{
		shader->SetUniform(depthTexName, depthTexture);
	}
	
//	shader->SetUniform("cameraPosition", game->scene->mainCamera->owner->transform->GetWorldPosition());
//	shader->SetUniform("cameraDof", vec2(game->scene->mainCamera->dof - game->scene->mainCamera->dofWidth, game->scene->mainCamera->dof + game->scene->mainCamera->dofWidth));
	shader->SetUniform("focalDepth", game->scene->mainCamera->focalDepth);
	shader->SetUniform("focalLength", game->scene->mainCamera->focalLength);
	shader->SetUniform("fstop", game->scene->mainCamera->aperture);
	shader->SetUniform("focus", game->scene->mainCamera->focus);
	shader->SetUniform("textureSize", (vec2)game->renderer->GetResolution());
	shader->SetUniform("cameraRange", vec2_cast<vec2>(game->scene->mainCamera->GetViewRange()));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, posBuffer.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	for(uint i = 0; i < textures.size(); ++i)
	{
		textures[i]->Unbind();
	}

	shader->Unbind();
}

void FrameBuffer::EndRender()
{
	posBuffer.Destroy();
	shader->Destroy();
}

}