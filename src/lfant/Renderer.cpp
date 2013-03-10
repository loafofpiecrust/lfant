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
#include <GL/glfw.h>

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

#define OFFSET(i) ((byte*)0 + (i))

namespace lfant
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

/*******************************************************************************
*
*		Game Loop
*
*******************************************************************************/

void Renderer::Init()
{
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
	}

	HideMouse(hideMouse);

	// Background color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Fragment depth testing
	glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
	//glDepthRange(0.0f, 1.0f);

	// Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Texture and shading
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Point sprites
	//glEnable(GL_POINT_SPRITE);
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//glEnable(GL_BLEND);

	//glfwSwapInterval(vSync);

	glfwSetWindowCloseCallback(&Renderer::WindowClosed);
	glfwSetWindowSizeCallback(&Renderer::WindowResized);

	Log("Renderer: Initialized");

	// Load all shaders
	/*
	   auto shs = game->fileSystem->GetGameFiles("shaders", "vert");
	   for(uint i = 0; i < shs.size(); ++i)
	   {
	        Shader shader;
	        shader.LoadFile(shs[i].string());
	        shaders.push_back(shader);
	   }*/
}

void Renderer::Update()
{
	glfwSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*******************************************************************************
*
*		Windowing
*
*******************************************************************************/

bool Renderer::OpenWindow()
{
	Log("Renderer::OpenWindow: About to set window hints.");
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, fsaa);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, version.major);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, version.minor);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, !windowResizable);

	Log("Renderer::OpenWindow: Window hints set.");

	int mode = GLFW_WINDOW;
	if(fullscreen)
	{
		mode = GLFW_FULLSCREEN;
	}

	Log("Renderer::OpenWindow: Window mode determined.");

	ivec2 res = GetResolution();
	if( !glfwOpenWindow( res.x, res.y, 0, 0, 0, 0, 32, 0, mode ) )
	{
		Log( "Renderer::OpenWindow: Failed to open GLFW window." );
		glfwTerminate();
		return false;
	}
	Log("Renderer::OpenWindow: Window opened.");

	glewExperimental = true;     // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		Log("Renderer::OpenWindow: Failed to initialize GLEW.");
		return false;
	}
	Log("Renderer::OpenWindow: GLEW Initialised.");

	glfwSetWindowTitle(game->settings->GetValue("general.windowtitle").c_str());

	Log("Renderer::OpenWindow: Window successfully opened.");

	return true;
}

int Renderer::WindowClosed()
{
	Log("Renderer::WindowClosed: Touch.");
	game->Exit();
	return 1;
}

void Renderer::WindowResized(int x, int y)
{
	Log("Renderer::WindowResized: Touch.");
	game->renderer->resolution = ivec2(x, y);
	glViewport(0, 0, x, y);
	game->userInterface->OnWindowResize((uint)x, (uint)y);
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

void Renderer::SetResolution(ivec2 res)
{
	resolution = res;
	glfwSetWindowSize(res.x, res.y);
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

void Renderer::SetPosition(ivec2 pos)
{
	glfwSetWindowPos(pos.x, pos.y);
}

Shader& Renderer::GetShader(string name)
{
	for(auto& shader : shaders)
	{
		if(shader.path == name)
		{
			return shader;
		}
	}
	return null(Shader);
}

void Renderer::AddShader(Shader& shader)
{
	if(Shader* s = &GetShader(shader.path))
	{
		s->id = shader.id;
		return;
	}
	shaders.push_back(shader);
}

void Renderer::HideMouse(bool hide)
{
	if(hide == true)
	{
		glfwDisable(GLFW_MOUSE_CURSOR);
	}
	else
	{
		glfwEnable(GLFW_MOUSE_CURSOR);
	}
}

}
