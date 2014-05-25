/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Renderer.h>

// External
#include <iostream>
#include <fstream>

#include <GL/glew.h>
//#include <GL/gl.h>
//#include <SFML/Graphics.hpp>
#if !ANDROID
//#include <GLFW/glfw3.h>
#endif

#include <glm/gtc/type_ptr.hpp>

// Internal

#include <lfant/Entity.h>
#include <lfant/Shader.h>

#include <lfant/Game.h>
#include <lfant/SystemInfo.h>
#include <lfant/Mesh.h>

#include <lfant/Console.h>
#include <lfant/Mesh.h>
#include <lfant/Sprite.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/FileSystem.h>
#include <lfant/UserInterface.h>
#include <lfant/Physics.h>
#include <lfant/Input.h>
#include "lfant/Window.h"

#include <lfant/TextureLoader.h>
#include <lfant/MeshLoader.h>
#include <lfant/Properties.h>
#include <lfant/FrameBuffer.h>
#include <lfant/Transform.h>
#include <lfant/Thread.h>
#include <lfant/Light.h>

#include <AntTweakBar.h>

namespace lfant {

Renderer::Renderer(Game* game) :
	Subsystem(game)
{
}

Renderer::~Renderer()
{
}

void Renderer::Serialize(Properties* prop)
{
	Subsystem::Serialize(prop);
//	GetGame()->Log("Renderer::Serialize: Got root child, '", prop->Value(""), "'.");

	prop->Value("vsync", &vsync);
	prop->Value("motionBlur", &motionBlur);

//	GetGame()->Log("Window title: '"+windowTitle+"'.");
//	GetGame()->Log("OpenGL Version loaded: ", lexical_cast<string>(version));
}

/*******************************************************************************
*
*		Game Loop
*
*******************************************************************************/

ivec2 res;
void Renderer::Init()
{
	LoadFile("settings/renderer.prop");

//	GetGame()->Log("Renderer::Init: About to start GLFW");

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Backface culling
//	glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CCW);

	// Texture and shading
	glEnable(GL_TEXTURE_2D);

	// Point sprites
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);

//	glEnable(GL_TEXTURE_RECTANGLE);

	GetGame()->Log("Renderer: Initialized");

	frameBuffer = new FrameBuffer(GetGame());
	frameBuffer->AddTexture("diffuseTex", Texture::Format::RGBA, Texture::Format::RGBA);
	frameBuffer->AddTexture("positionTex", Texture::Format::RGB32F, Texture::Format::RGB);
	frameBuffer->AddTexture("normalTex", Texture::Format::RGB32F, Texture::Format::RGB);
	frameBuffer->AddTexture("lightTex", Texture::Format::RGBA, Texture::Format::RGBA);
//	frameBuffer->AddTexture("specularTex", Texture::Format::RGBA, Texture::Format::RGBA);
	frameBuffer->AddDepthTexture("depthTex");

	res = game->window->GetSize();
	GetGame()->Log("making framebuffer of size ", res);
	frameBuffer->SetRect({0,0,(uint32)res.x,(uint32)res.y});
	frameBuffer->Init();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load all shaders
	/*
	   auto shs = game->fileSystem->GetGameFiles("shaders", "vert");
	   for(uint i = 0; i < shs.size(); ++i)
	   {
			Shader shader;
			shader->LoadFile(shs[i].string());
			shaders.push_back(shader);
	   }*/


}

TwBar* bar = nullptr;

void Renderer::Update()
{
	// outline:
	//
	// flip buffers
	// bind fbo
	// glDrawBuffers(main/all textures)
	// draw scene
	// glDrawBuffers(none)
	// draw stencil for lights
	// glDrawBuffers(light texture)
	// bind fbo textures
	// draw lights
	// unbind fbo
	// draw fbo quad
	//

	GetGame()->window->Render();

	frameBuffer->Bind();
	frameBuffer->DrawBuffers();
	Resize(res);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);

	static bool physicsSwitch = false;
	if(GetGame()->input->GetButtonDown("SwitchRendering") == 1)
	{
		physicsSwitch = !physicsSwitch;
		std::cout << "switch\n";
	}

	if(physicsSwitch)
	{
		GetGame()->physics->Render();
	}
	else
	{
		GetGame()->scene->Render();
	}


	if(lights.size() > 0)
	{
		glDrawBuffer(GL_NONE);

		//
		// stencil pass
		//
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		for(auto& l : lights)
		{
			l->RenderStencil();
		}

		//
		// light pass
		//
		frameBuffer->DrawBuffer(3);
		frameBuffer->BindTextures();

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		for(auto& l : lights)
		{
			l->Render();
		}

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
		glDisable(GL_STENCIL_TEST);
	}

	Resize(res);

	//
	// final pass
	//
	frameBuffer->Unbind();
	frameBuffer->Render();

	if(GetGame()->scene->mainCamera)
	{
		if(!bar)
		{
			ConnectEvent(SENDER(GetGame()->input.get(), MouseMove), RECEIVER(this, OnMouseMove));
			ConnectEvent(SENDER(GetGame()->input.get(), MouseButton), RECEIVER(this, OnMouseButton));

			TwInit(TW_OPENGL, nullptr);
			auto size = GetGame()->window->GetSize();
			TwWindowSize(size.x, size.y);

			bar = TwNewBar("Camera DoF Settings");
			TwAddVarRW(bar, "dof", TW_TYPE_BOOLCPP, &GetGame()->scene->mainCamera->useDof, "");
			TwAddVarRW(bar, "autoFocus", TW_TYPE_BOOLCPP, &GetGame()->scene->mainCamera->autoFocus, "");
			TwAddVarRW(bar, "focalLength", TW_TYPE_FLOAT, &GetGame()->scene->mainCamera->focalLength, "");
			TwAddVarRW(bar, "focalDepth", TW_TYPE_FLOAT, &GetGame()->scene->mainCamera->focalDepth, "");
			TwAddVarRW(bar, "aperture", TW_TYPE_FLOAT, &GetGame()->scene->mainCamera->aperture, "");
			TwAddVarRW(bar, "focus", TW_TYPE_FLOAT, &GetGame()->scene->mainCamera->focus, "");
		}
		else
		{
			TwDraw();
		}
	}
}

void Renderer::Deinit()
{
	TwTerminate();
	frameBuffer->Deinit();
//	Texture::textureCache.clear();
	GetGame()->Log("Renderer::Deinit(): Touch");
}

/*******************************************************************************
*
*		Gets/Sets/Shaders
*
*******************************************************************************/

void Renderer::Resize(ivec2 size)
{
	glViewport(0,0,size.x,size.y);
}

void Renderer::SetRendering(bool render)
{
	if(!render && render)
	{
		Update();
	}
	this->render = render;
}

Shader *Renderer::GetShader(string name)
{
//	for(auto& shader : shaders)
	{
//		if(shader->path == name)
		{
//			return shader;
		}
	}
	return nullptr;
}

void Renderer::AddShader(Shader *shader)
{
//	if(Shader* s = &GetShader(shader->path))
	{
//		s->id = shader->id;
//		return;
	}
//	shaders.push_back(ptr<Shader>(shader));
}

void Renderer::AddLight(Light* light)
{
	lights.push_back(light);
}

void Renderer::RemoveLight(Light* light)
{
	for(uint i = 0; i < lights.size(); ++i)
	{
		if(lights[i] == light)
		{
			lights.erase(lights.begin()+i);
		}
	}
}

void Renderer::OnMouseMove(ivec2 pos)
{
	TwEventMousePosGLFW(pos.x, pos.y);
}

void Renderer::OnMouseButton(uint16 button, int action)
{
	std::cout << "mussolinni\n";
	TwEventMouseButtonGLFW((int)button, action);
}

}
