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
//#include <GL/gl.h>
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
#include "FileSystem.hpp"
#include "Settings.hpp"
#include "UserInterface.hpp"

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

void Renderer::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	if( !glfwOpenWindow( res.x, res.y, 0,0,0,0,32,0, mode ) )
	{
		Log( "Renderer::OpenWindow: Failed to open GLFW window." );
		glfwTerminate();
		return false;
	}
	Log("Renderer::OpenWindow: Window opened.");

	glewExperimental = true; // Needed for core profile
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
	game->renderer->resolution = ivec2(x,y);
	glViewport(0, 0, x, y);
	game->userInterface->OnWindowResize((uint)x, (uint)y);
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

	Log("About to add mesh");

	bool shaderLoaded = false;
	if(mesh->material.shader.id == 0)
	{
		if(mesh->material.shader.name != "")
		{
			for(auto& shader : shaders)
			{
				if(shader.name == mesh->material.shader.name && shader.id != 0)
				{
					mesh->material.shader.id = shader.id;
					shaderLoaded = true;
					break;
				}
			}
		}
		else
		{
			for(auto& shader : shaders)
			{
				if(shader.name == "shaders/Diffuse" && shader.id != 0)
				{
					mesh->material.shader.id = shader.id;
					shaderLoaded = true;
					break;
				}
			}
		}
	}
	if(!shaderLoaded)
	{
		Log("Loading default shader");
		mesh->material.shader.LoadFile("shaders/Diffuse");
		shaders.push_back(mesh->material.shader);
	}

	Log(mesh->material.texture.id);

	if(mesh->material.texture.id == 0)
	{
		Log(mesh->material.texture.name);
		mesh->material.texture.LoadFile(mesh->material.texture.name);
	}

	Log("Renderer::AddMesh: Texture loaded");

	if(mesh->material.shader.id != 0)
	{
		// Get any uniforms here
		mesh->matrixId = glGetUniformLocation(mesh->material.shader.id, "MVP");
		mesh->material.texture.uniformId = glGetUniformLocation(mesh->material.shader.id, "textureSampler");
	}

	Log("Renderer::AddMesh: Uniforms loaded");

	vector<uint32_t> indices;
	vector<vec3> indexed_vertices;
	vector<vec2> indexed_uvs;
	vector<vec3> indexed_normals;
	IndexVBO(mesh->vertexBuffer.data, mesh->uvBuffer.data, mesh->normalBuffer.data, indices, indexed_vertices, indexed_uvs, indexed_normals);

	mesh->vertexBuffer.data = indexed_vertices;
	mesh->uvBuffer.data = indexed_uvs;
	mesh->normalBuffer.data = indexed_normals;
	mesh->indexBuffer.data = indices;

	Log("Renderer::AddMesh: Data indexed");

	mesh->vertexBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer.data);
	mesh->uvBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, mesh->uvBuffer.data);
	mesh->normalBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer.data);
	mesh->indexBuffer.id = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer.data);

	Log("Renderer::AddMesh: Data buffered");
}

void Renderer::RenderMesh(Mesh* mesh)
{
	if(mesh->material.shader.id == 0 || mesh->material.texture.id == 0)
	{
		AddMesh(mesh);
	}

	glBindVertexArray(mesh->vertexArray);

	glUseProgram(mesh->material.shader.id);

	mat4 mvp = game->scene->mainCamera->projection * game->scene->mainCamera->view * mesh->transform->GetMatrix();
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
	glDeleteBuffers(1, &mesh->vertexBuffer.id);
	glDeleteBuffers(1, &mesh->uvBuffer.id);
	glDeleteBuffers(1, &mesh->normalBuffer.id);
	glDeleteBuffers(1, &mesh->indexBuffer.id);
	glDeleteTextures(1, &mesh->material.texture.id);
	glDeleteVertexArrays(1, &mesh->vertexArray);
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
	glGenVertexArrays(1, &sprite->vertexArray);
	glBindVertexArray(sprite->vertexArray);

	Log("About to add sprite");

	bool shaderLoaded = false;
	if(sprite->material.shader.id == 0)
	{
		if(sprite->material.shader.name != "")
		{
			for(auto& shader : shaders)
			{
				if(shader.name == sprite->material.shader.name && shader.id != 0)
				{
					sprite->material.shader.id = shader.id;
					shaderLoaded = true;
					break;
				}
			}
		}
		else
		{
			for(auto& shader : shaders)
			{
				if(shader.name == "shaders/Diffuse" && shader.id != 0)
				{
					sprite->material.shader.id = shader.id;
					shaderLoaded = true;
					break;
				}
			}
		}
	}
	if(!shaderLoaded)
	{
		Log("Loading default shader");
		sprite->material.shader.LoadFile("shaders/Diffuse");
		shaders.push_back(sprite->material.shader);
	}

	Log(sprite->material.texture.id);

	if(sprite->material.texture.id == 0)
	{
		Log(sprite->material.texture.name);
		sprite->material.texture.LoadFile(sprite->material.texture.name);
	}

	Log("Renderer::AddSprite: Texture loaded");

	if(sprite->material.shader.id != 0)
	{
		// Get any uniforms here
		sprite->matrixId = glGetUniformLocation(sprite->material.shader.id, "MVP");
		sprite->material.texture.uniformId = glGetUniformLocation(sprite->material.shader.id, "textureSampler");
	}

	Log("Renderer::AddSprite: Uniforms loaded");

	sprite->vertexBuffer.push_back(vec3(0, 0, 0));
	sprite->vertexBuffer.push_back(vec3(1, 0, 0));
	sprite->vertexBuffer.push_back(vec3(1, 1, 0));
	sprite->vertexBuffer.push_back(vec3(0, 1, 0));

	sprite->uvBuffer.push_back(vec2(0, 0));
	sprite->uvBuffer.push_back(vec2(1, 0));
	sprite->uvBuffer.push_back(vec2(1, 1));
	sprite->uvBuffer.push_back(vec2(0, 1));

	sprite->indexBuffer.push_back(0);
	sprite->indexBuffer.push_back(1);
	sprite->indexBuffer.push_back(2);
	sprite->indexBuffer.push_back(3);

	/*
	vector<uint32_t> indices;
	vector<vec3> indexed_vertices;
	vector<vec2> indexed_uvs;
	IndexVBO(sprite->vertexBuffer.data, sprite->uvBuffer.data, indices, indexed_vertices, indexed_uvs);

	sprite->vertexBuffer.data = indexed_vertices;
	sprite->uvBuffer.data = indexed_uvs;
	sprite->indexBuffer.data = indices;
	*/

	Log("Renderer::AddSprite: Data indexed");

	sprite->vertexBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, sprite->vertexBuffer.data);
	sprite->uvBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, sprite->uvBuffer.data);
	sprite->indexBuffer.id = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->indexBuffer.data);

	Log("Renderer::AddSprite: Data buffered");

}

void Renderer::RenderSprite(Sprite* sprite)
{
	if(sprite->material.shader.id == 0 || sprite->material.texture.id == 0)
	{
		AddSprite(sprite);
	}

	glBindVertexArray(sprite->vertexArray);

	glUseProgram(sprite->material.shader.id);

	mat4 mvp = game->scene->mainCamera->projection * game->scene->mainCamera->view * sprite->transform->GetMatrix();
	glUniformMatrix4fv(sprite->matrixId, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->material.texture.id);
	glUniform1i(sprite->material.texture.uniformId, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vertexBuffer.id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->uvBuffer.id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/*
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->normalBuffer.id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->indexBuffer.id);
	glDrawElements(GL_QUADS, sprite->indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
}

void Renderer::RemoveSprite(Sprite* sprite)
{
	glDeleteBuffers(1, &sprite->vertexBuffer.id);
	glDeleteBuffers(1, &sprite->uvBuffer.id);
	glDeleteBuffers(1, &sprite->indexBuffer.id);
	glDeleteTextures(1, &sprite->material.texture.id);
	glDeleteVertexArrays(1, &sprite->vertexArray);
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
