/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-17 by Taylor Snead
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

#include <lfant/Renderer.h>

// External
#include <iostream>
#include <fstream>

#include <GL/glew.h>
//#include <GL/gl.h>
//#include <SFML/Graphics.hpp>
#include <GLFW/glfw3.h>

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
#include <lfant/Settings.h>
#include <lfant/UserInterface.h>

#include <lfant/TextureLoader.h>
#include <lfant/MeshLoader.h>
#include <lfant/Properties.h>
#include <lfant/FrameBuffer.h>
#include <lfant/Transform.h>
#include <lfant/Thread.h>

#define OFFSET(i) ((byte*)0 + (i))

namespace lfant
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Load(Properties* prop)
{
	Subsystem::Load(prop);
	Log("Renderer::Load: Got root child, '", prop->id, "'.");

	prop->Get("resolution", resolution);
	prop->Get("version", version);
	prop->Get("vsync", vsync);
	prop->Get("fullscreen", fullscreen);
	prop->Get("fsaa", fsaa);
	prop->Get("windowResizable", windowResizable);
	prop->Get("windowPos", windowPos);
	prop->Get("windowTitle", windowTitle);

	Log("Window title: '"+windowTitle+"'.");
}

void Renderer::Save(Properties *prop)
{
	Subsystem::Save(prop);

	prop->Set("resolution", resolution);
	prop->Set("version", version);
	prop->Set("vsync", vsync);
	prop->Set("fullscreen", fullscreen);
	prop->Set("fsaa", fsaa);
	prop->Set("windowResizable", windowResizable);
	prop->Set("windowTitle", windowTitle);
}

/*******************************************************************************
*
*		Game Loop
*
*******************************************************************************/

void Renderer::Init()
{
	Subsystem::Init();

	Log("Renderer::Init: About to start GLFW");
	if(!glfwInit())
	{
		Log("Renderer::Init: GLFW failed to initialise.");
		game->Exit();
	}

	if(game->standAlone)
	{
		if(!OpenWindow())
		{
			// Window opening failed
			game->Exit();
		}

		HideMouse(hideMouse);
	}


	// Background color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Fragment depth testing
	glDepthFunc(GL_LEQUAL);
//	glDepthMask(GL_FALSE);
//	glDepthRange(0.0f, 1.0f);

	// Backface culling
	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);

	// Texture and shading
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Point sprites
//	glEnable(GL_POINT_SPRITE);
//	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glEnable(GL_TEXTURE_RECTANGLE);

	if(game->standAlone)
	{
		glfwSwapInterval(vsync);

		glfwSetWindowCloseCallback(window, &Renderer::OnCloseWindow);
		glfwSetWindowSizeCallback(window, &Renderer::OnSetResolution);
	}

	Log("Renderer: Initialized");

	{
		Mesh* mesh = game->scene->Spawn("Lala")->AddComponent<Mesh>();
		mesh->LoadFile("meshes/suzanne.obj");
		mesh->material->LoadFile("materials/Diffuse.mat");
	}
	
	
	fboEntity = game->scene->Spawn("FBOQuad");
	fboEntity->transform->SetPosition(vec3(0.1,0.1,1));
	fboEntity->active = false;
//	fboEntity->transform->SetScale()

	fboQuad = fboEntity->AddComponent<Mesh>();
	fboQuad->Enable(false);
//	fboQuad = new Mesh;
	fboQuad->usingCamera = false;
//	fboQuad->Init();
	fboShader = fboQuad->material->shader;
	fboQuad->material->loaded = true;
	fboShader->LoadFile("shaders/FrameBuffer");
	Log("Shader for fbo: ", fboShader->GetId());
	fboQuad->LoadFile("meshes/quad.obj");
	Log("Shader for fbo: ", fboShader->GetId());
//	fboQuad->material->texture->LoadFile("textures/Default.png");
//	fboQuad->material->texture->size = uvec2(1024,768);
//	fboQuad->material->shader->AddUniform("matrix");
	
	frameBuffer = new FrameBuffer();
	frameBuffer->AddTexture("textureSampler", fboQuad->material->texture);
	Log("Shader for fbo: ", fboShader->GetId());
//	fboQuad->material->texture->Init();
//	frameBuffer->AddTexture("texThreshold");
	frameBuffer->SetRect({0,0,resolution.x, resolution.y});
	frameBuffer->hasDepth = true;
	frameBuffer->Init();
	Log("Shader for fbo: ", fboShader->GetId());

	frameBuffer->Bind();
	glViewport(0,0,1920, 1080);
	frameBuffer->Clear();
	
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

void Renderer::Update()
{

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);

//	fboQuad->Render();

	// Rendering to the framebuffer
	
//	frameBuffer->Bind();
	if(!fboDrawn)
	{
//		thread::Sleep(1000);
//		frameBuffer->Render();
//		Log("fbo drawn once.");
//		thread::Sleep(1000);
		fboDrawn = true;
	}

	glfwSwapBuffers(window);
	glfwPollEvents();

	frameBuffer->Unbind();
//	glViewport(0,0,1024, 768);
	glViewport(0,0,resolution.x,resolution.y);
//	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	frameBuffer->GetTextures(fboShader);

//	fboQuad->material->shader->Bind();
	fboQuad->Render();
//	fboQuad->material->shader->Unbind();

	frameBuffer->Bind();
//	glViewport(0,0,1024, 768);
//	glViewport(0,0,resolution.x,resolution.y);
//	glClearColor(0.0f, 0.4f, 0.0f, 0.0f);
	frameBuffer->Clear();

//	fboQuad->Render();
//	frameBuffer->Unbind();
}

void Renderer::OnDestroy()
{
//	fboQuad->OnDestroy();
//	delete fboQuad;
	Log("Renderer::OnDestroy(): Touch");
	glfwTerminate();
}

/*******************************************************************************
*
*		Windowing
*
*******************************************************************************/

bool Renderer::OpenWindow()
{
//	if(game->standAlone)
//	{
	Log("Renderer::OpenWindow: About to set window hints.");
	glfwWindowHint(GLFW_SAMPLES, fsaa);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	Log("Renderer::OpenWindow: Window successfully opened.");
//	}

	glewExperimental = true;     // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		Log("Renderer::OpenWindow: Failed to initialize GLEW.");
		return false;
	}
	Log("Renderer::OpenWindow: GLEW Initialised.");

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
	game->renderer->TriggerEvent("SetResolution", (uint32)x, (uint32)y);
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

void Renderer::SetWindowTitle(string title)
{
	windowTitle = title;
	glfwSetWindowTitle(window, title.c_str());
}

void Renderer::SetResolution(ivec2 res)
{
	resolution = res;
	glfwSetWindowSize(window, res.x, res.y);
}

void Renderer::SetVersion(byte major, byte minor)
{
	version = {major, minor};
	if(!OpenWindow())
	{
		game->Exit();
	}
}

void Renderer::SetRendering(bool render)
{
	if(!render && render)
	{
		Update();
	}
	this->render = render;
}

void Renderer::SetWindowPos(ivec2 pos)
{
	windowPos = pos;
	glfwSetWindowPos(window, pos.x, pos.y);
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

void Renderer::HideMouse(bool hide)
{
	glfwSetInputMode(window, GLFW_CURSOR, !hide);
}

}
