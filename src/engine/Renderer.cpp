/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include "Renderer.hpp"

// External
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/gl.h>
//#include <SFML/Graphics.hpp>
#include <GL/glfw.h>

#include <glm/gtc/type_ptr.hpp>

// Internal
#include "Entity.hpp"
#include "Shader.hpp"
#include "Engine.hpp"
#include "SystemInfo.hpp"
#include "Mesh.hpp"
#include "Console.hpp"
#include "Mesh.hpp"
#include "ParticleSystem.hpp"
#include "Sprite.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "FileManager.hpp"
#include "Settings.hpp"

#include "TextureLoader.hpp"
#include "MeshLoader.hpp"

#define OFFSET(i) ((byte*)0 + (i))

namespace sfs
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
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	// Texture and shading
	glEnable(GL_TEXTURE_2D);
	//glShadeModel(GL_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Point sprites
	//glEnable(GL_POINT_SPRITE);
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//glEnable(GL_BLEND);

	//glfwSwapInterval(vSync);

	glfwSetWindowCloseCallback(&Renderer::WindowClosed);
	glfwSetWindowSizeCallback(&Renderer::WindowResized);

	Log("Renderer: Initialized");

	// Load all shaders
	auto shs = game->fileManager->GetGameFiles("shaders", "vert");
	for(uint i = 0; i < shs.size(); ++i)
	{
		Shader shader;
		shader.LoadFile(shs[i].string());
		shaders.push_back(shader);
	}
}

void Renderer::PreUpdate()
{
}

void Renderer::Update()
{
	glfwSwapBuffers();
}

/*******************************************************************************
 *
 *		Windowing
 *
 *******************************************************************************/

bool Renderer::OpenWindow()
{
	/*
	int mode = sf::Style::Default;
	if(fullscreen)
	{
		mode = sf::Style::Fullscreen || sf::Style::None;
	}

	sf::ContextSettings settings;
	settings.majorVersion = version.major;
	settings.minorVersion = version.minor;
	settings.antialiasingLevel = fsaa;

	window = new sf::RenderWindow(sf::VideoMode(resolution().x, resolution().y), game->settings->GetValue("windowtitle").s(), mode, settings);
	if(window)
	{
		return true;
	}
	*/

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, fsaa);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, version.major);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, version.minor);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int mode = GLFW_WINDOW;
	if(fullscreen)
	{
		mode = GLFW_FULLSCREEN;
	}

	ivec2 res = GetResolution();
	if( !glfwOpenWindow( res.x, res.y, 0,0,0,0, 32,0, mode ) )
	{
		Log( "Renderer::OpenWindow: Failed to open GLFW window." );
		glfwTerminate();
		return false;
	}

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		Log("Renderer::OpenWindow: Failed to initialize GLEW.");
		return false;
	}

	glfwSetWindowTitle(game->settings->GetValue("general.windowtitle").c_str());

	Log("Renderer::OpenWindow: Window successfully opened.");

	return true;
}

int Renderer::WindowClosed()
{
	Log("Window closed");
	game->Exit();
	return 1;
}

void Renderer::WindowResized(int x, int y)
{
	game->renderer->resolution = ivec2(x,y);
}

/*******************************************************************************
 *
 *		General Mesh Rendering
 *
 *******************************************************************************/

template<typename T>
uint32_t Renderer::CreateBuffer(int target, vector<T>& data, int mode)
{
	if(mode == 0)
	{
		mode = GL_STATIC_DRAW;
	}
	uint32_t id;
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, data.size() * sizeof(T), &data[0], mode);
	return id;
}

uint32_t Renderer::CreateBuffer(int target, void* data, uint32_t size, int mode)
{
	if(mode == 0)
	{
		mode = GL_STATIC_DRAW;
	}
	uint32_t id;
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, mode);
	return id;
}

void Renderer::AddMesh(Mesh* mesh)
{
	glGenVertexArrays(1, &mesh->vertexArray);
	glBindVertexArray(mesh->vertexArray);

	if(mesh->material.shader.id == 0)
	{
		if(mesh->material.shader.name != "")
		{
			for(auto& shader : shaders)
			{
				if(shader.name == mesh->material.shader.name)
				{
					mesh->material.shader.id == shader.id;
				}
			}
		}
		if(mesh->material.shader.id == 0)
		{
			mesh->material.shader.LoadFile("../../assets/shaders/Diffuse");
		}
	}

	Log(mesh->material.texture.id);

	if(mesh->material.texture.id == 0)
	{
		Log(mesh->material.texture.name);
		mesh->material.texture.LoadFile(mesh->material.texture.name);
	}

	if(mesh->material.shader.id != 0)
	{
		// Get any uniforms here
		mesh->matrixId = glGetUniformLocation(mesh->material.shader.id, "MVP");
		mesh->material.texture.uniformId = glGetUniformLocation(mesh->material.shader.id, "textureSampler");
	}

	vector<uint32_t> indices;
	vector<vec3> indexed_vertices;
	vector<vec2> indexed_uvs;
	vector<vec3> indexed_normals;
	IndexVBO(mesh->vertexBuffer.data, mesh->uvBuffer.data, mesh->normalBuffer.data, indices, indexed_vertices, indexed_uvs, indexed_normals);

	mesh->vertexBuffer.data = indexed_vertices;
	mesh->uvBuffer.data = indexed_uvs;
	mesh->normalBuffer.data = indexed_normals;
	mesh->indexBuffer.data = indices;

	mesh->vertexBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer.data);
	mesh->uvBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, mesh->uvBuffer.data);
	mesh->normalBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer.data);
	mesh->indexBuffer.id = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer.data);
}

void Renderer::RenderMesh(Mesh* mesh)
{
	if(mesh->material.shader.id == 0 || mesh->material.texture.id == 0)
	{
		AddSprite(mesh);
	}

	glBindVertexArray(mesh->vertexArray);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mesh->material.shader.id);

	mat4 mvp = game->scene->mainCamera->projection * game->scene->mainCamera->view * mesh->transform->matrix;
	glUniformMatrix4fv(mesh->matrixId, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->material.texture.id);
	glUniform1i(mesh->material.texture.uniformId, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer.id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvBuffer.id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer.id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer.id);
	glDrawElements(GL_TRIANGLES, mesh->indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
}

void Renderer::RemoveMesh(Mesh* mesh)
{
	glDeleteBuffers(1, &sprite->vertexBuffer.id);
	glDeleteBuffers(1, &sprite->uvBuffer.id);
	glDeleteBuffers(1, &sprite->normalBuffer.id);
	glDeleteBuffers(1, &sprite->indexBuffer.id);
	glDeleteTextures(1, &sprite->material.texture.id);
	glDeleteVertexArrays(1, &sprite->vertexArray);
}

/*******************************************************************************
 *
 *		Particle System Rendering
 *
 *******************************************************************************/

void Renderer::AddParticles(ParticleSystem* system)
{
}

void Renderer::RenderParticles(ParticleSystem* system)
{
}

void Renderer::RemoveParticles(ParticleSystem* system)
{

}

/*******************************************************************************
 *
 *		Sprite Rendering
 *
 *******************************************************************************/

void Renderer::AddSprite(Sprite* sprite)
{

}

void Renderer::RenderSprite(Sprite* sprite)
{
}

void Renderer::RemoveSprite(Sprite* sprite)
{
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
}

void Renderer::SetRendering(bool render)
{
	if(!render && render)
	{
		Update();
	}
	this->render = render;
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
