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
#include <GLFW/glfw3.h>
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
#include <lfant/ParticleSystem.h>
#include <lfant/Sprite.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/FileSystem.h>
#include <lfant/UserInterface.h>

#include <lfant/TextureLoader.h>
#include <lfant/MeshLoader.h>
#include <lfant/Properties.h>
#include <lfant/FrameBuffer.h>
#include <lfant/Transform.h>
#include <lfant/Thread.h>
#include <lfant/Light.h>

namespace lfant {

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Load(Properties* prop)
{
	Subsystem::Load(prop);
//	Log("Renderer::Load: Got root child, '", prop->Get(""), "'.");

	prop->Get("resolution", resolution);
	prop->Get("version", version);
	prop->Get("vsync", vsync);
	prop->Get("fullscreen", fullscreen);
	prop->Get("fsaa", fsaa);
	prop->Get("windowResizable", windowResizable);
	prop->Get("windowPos", windowPos);
	prop->Get("windowTitle", windowTitle);
	prop->Get("motionBlur", motionBlur);

	Log("Window title: '"+windowTitle+"'.");
	Log("OpenGL Version loaded: ", lexical_cast<string>(version));
}

void Renderer::Save(Properties *prop) const
{
	Subsystem::Save(prop);

	prop->Set("resolution", resolution);
	prop->Set("version", version);
	prop->Set("vsync", vsync);
	prop->Set("fullscreen", fullscreen);
	prop->Set("fsaa", fsaa);
	prop->Set("windowResizable", windowResizable);
	prop->Set("windowTitle", windowTitle);
	prop->Set("motionBlur", motionBlur);
}

/*******************************************************************************
*
*		Game Loop
*
*******************************************************************************/
/*
void Renderer::OnError(uint source, uint type, uint id, uint severity, int length, const char* message, void* user)
{
	Log("OpenGL Error (", severity, "): ", message);
}*/

void Renderer::OnError(int error, const char* msg)
{
	Log("GLFW Error(", error, "): '", msg, "'.");
}

void Renderer::Init()
{
	Subsystem::Init();

	Log("Renderer::Init: About to start GLFW");

#if !ANDROID
	if(game->standAlone)
	{
		if(!glfwInit())
		{
			Log("Renderer::Init: GLFW failed to initialise.");
			game->Destroy();
		}
	}
#endif

//	if(game->standAlone)
	{
		if(!OpenWindow())
		{
			// Window opening failed
			game->Destroy();
		}

		HideMouse(hideMouse);
	}

#if !ANDROID

	if(game->standAlone)
	{
		glfwSwapInterval(vsync);
		glfwSetWindowCloseCallback(window, &Renderer::OnCloseWindow);
		glfwSetWindowSizeCallback(window, &Renderer::OnSetResolution);
		glfwSetErrorCallback(&Renderer::OnError);
	}

#endif

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	// Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Texture and shading
	glEnable(GL_TEXTURE_2D);

	// Point sprites
//	glEnable(GL_POINT_SPRITE);
//	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glEnable(GL_DEBUG_OUTPUT);
//	glDebugMessageCallback(OnError, 0);

//	glEnable(GL_TEXTURE_RECTANGLE);

	Log("Renderer: Initialized");

	frameBuffer = new FrameBuffer();
	frameBuffer->AddTexture("lightTex", Texture::Format::RGBA, Texture::Format::RGBA);
	frameBuffer->AddTexture("diffuseTex", Texture::Format::RGBA, Texture::Format::RGBA);
	frameBuffer->AddTexture("positionTex", Texture::Format::RGB32F, Texture::Format::RGB);
	frameBuffer->AddTexture("normalTex", Texture::Format::RGB32F, Texture::Format::RGB);
//	frameBuffer->AddTexture("specularTex", Texture::Format::RGBA, Texture::Format::RGBA);
	frameBuffer->AddDepthTexture("depthTex");

	frameBuffer->SetRect({0,0,(uint32_t)resolution.x,(uint32_t)resolution.y});
//	frameBuffer->hasDepth = true;
	frameBuffer->Init();
	frameBuffer->Bind();
	frameBuffer->BeginRender();
	frameBuffer->Unbind();


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

static bool fboDrawn = false;

void Renderer::PreUpdate()
{
#if !ANDROID
	if(game->standAlone)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	glEnable(GL_DEPTH_TEST);
	frameBuffer->Bind();
	glViewport(0,0,resolution.x,resolution.y);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Update()
{
	if(lights.size() > 0)
	{
	//	frameBuffer->Unbind();
	//	glDepthMask(GL_FALSE);
	//	glDisable(GL_DEPTH_TEST);
	//	glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		for(auto& light : lights)
		{
			light->Render();
		}
	//	frameBuffer->UnbindRead();

	//	glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	glEnable(GL_DEPTH_TEST);
	}

//	glEnable(GL_DEPTH_TEST);

	// Rendering to the framebuffer

	frameBuffer->Unbind();
	glViewport(0,0,resolution.x,resolution.y);

	frameBuffer->Render();
}

void Renderer::Deinit()
{
	frameBuffer->Deinit();
	Log("Renderer::Deinit(): Touch");
#if !ANDROID
	if(game->standAlone) glfwTerminate();
#endif
}

/*******************************************************************************
*
*		Windowing
*
*******************************************************************************/

bool Renderer::OpenWindow()
{
#if !ANDROID
	if(game->standAlone)
	{
		Log("Renderer::OpenWindow: About to set window hints.");
		glfwWindowHint(GLFW_SAMPLES, fsaa);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, windowResizable);

		Log("Renderer::OpenWindow: Window hints set.");

		ivec2 res = GetResolution();
		if( !(window = glfwCreateWindow( res.x, res.y, windowTitle.c_str(), nullptr, nullptr )) )
		{
			Log( "Renderer::OpenWindow: Failed to open GLFW window." );
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		Log("Renderer::OpenWindow: Window opened.");

	//	SetWindowTitle(windowTitle);
		SetWindowPos(windowPos);
		OnSetResolution(window, res.x, res.y);
		Log("Renderer::OpenWindow: Window successfully opened.");
	}
	glewExperimental = true;     // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		Log("Renderer::OpenWindow: Failed to initialize GLEW.");
		return false;
	}
	Log("Renderer::OpenWindow: GLEW Initialised.");
	
	
#endif

	return true;
}

void Renderer::OnCloseWindow(GLFWwindow* win)
{
	Log("Renderer::WindowClosed: Touch.");
	game->Exit();
}

void Renderer::OnSetResolution(GLFWwindow* win, int x, int y)
{
//	Log("Renderer::OnSetResolution: Touch.");
	game->renderer->resolution = ivec2(x, y);
	/*
	game->renderer->frameBuffer->Unbind();
	glViewport(0, 0, x, y);
	game->renderer->frameBuffer->Bind();
	glViewport(0, 0, x, y);
	game->renderer->frameBuffer->rect.width = x;
	game->renderer->frameBuffer->rect.height = y;
	game->renderer->frameBuffer->Render();
//	frameBuffer->ResizeViewport();
	*/
	game->renderer->TriggerEvent("SetResolution", x, y);
}

/*******************************************************************************
*
*		Gets/Sets/Shaders
*
*******************************************************************************/

ivec2 Renderer::GetResolution()
{
	return resolution;
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

}
