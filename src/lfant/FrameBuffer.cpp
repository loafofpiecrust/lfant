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

// External
#include <GL/glew.h>

namespace lfant {

FrameBuffer* FrameBuffer::current;

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Init()
{
	glGenFramebuffers(1, &id);
	Bind();

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
			tex->internalFormat = GL_RGB32F;
			tex->format = GL_RGBA;
			tex->magFilter = GL_LINEAR;
		//	tex->mode = GL_TEXTURE_2D_MULTISAMPLE;
		//	tex->msaa = 2;
			tex->size.x = rect.width;
			tex->size.y = rect.height;
			tex->SetIndex(i);
			tex->InitData(nullptr);

			uint32_t attachment = GetAttachment(i);
			Log("Setting up textures for FrameBuffer, tex ", i, " gets attachment ", attachment, ". GL_TEXTURE0 = ", GL_COLOR_ATTACHMENT0);

			tex->Bind();
		//	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex->GetMode(), tex->GetId(), 0);
			glFramebufferTexture(GL_FRAMEBUFFER, attachment, tex->GetId(), 0);
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
	//	drawBuffers.push_back(GL_DEPTH_ATTACHMENT);
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
	Render();
	Unbind();
}

void FrameBuffer::OnDestroy()
{
	for(auto& tex : textures)
	{
		if(tex)
		{
			delete tex;
		}
	}
}

void FrameBuffer::AddTexture(string name, Texture* tex)
{
	if(tex)
	{
		textures.push_back(tex);
	}
	else
	{
		textures.push_back(new Texture);
	}
	textureNames.push_back(name);
}

void FrameBuffer::GetTextures(Shader* sh)
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

FrameBuffer* FrameBuffer::GetCurrent()
{
	return FrameBuffer::current;
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	FrameBuffer::current = this;
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	FrameBuffer::current = nullptr;
}

void FrameBuffer::Render()
{
	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log("Framebuffer failed to draw.");
	}
}

uint32_t FrameBuffer::GetAttachment(uint32_t idx)
{
	switch(idx)
	{
		case 1: return GL_COLOR_ATTACHMENT1;
		case 2: return GL_COLOR_ATTACHMENT2;
		case 3: return GL_COLOR_ATTACHMENT3;
		case 4: return GL_COLOR_ATTACHMENT4;
		case 5: return GL_COLOR_ATTACHMENT5;
		case 6: return GL_COLOR_ATTACHMENT6;
		case 7: return GL_COLOR_ATTACHMENT7;
		case 8: return GL_COLOR_ATTACHMENT8;
		case 9: return GL_COLOR_ATTACHMENT9;
		default: return GL_COLOR_ATTACHMENT0;
	}
}

void FrameBuffer::ResizeViewport()
{
	glViewport(rect.x, rect.y, rect.width, rect.height);
}

void FrameBuffer::Resize()
{

}

}