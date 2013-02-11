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

#include "ShaderLoader.hpp"
#include "TextureLoader.hpp"

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
	Log("Initializing Renderer.");

	if(!glfwInit())
	{
		Log("GLFW initialization failed.");
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
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	// Texture and shading
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Point sprites
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);

	glfwSwapInterval(vSync);

	Log("Renderer: Initialized");

	// Load all shaders
	auto shs = game->fileManager->GetGameFiles("shaders", "vert");
	for(uint i = 0; i < shs.size(); ++i)
	{
		shaders.push_back(LoadShader(shs[i].string()));
	}
}

void Renderer::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);
}

void Renderer::Update()
{
	// Swap screen buffers.
	glfwSwapBuffers();
}

/*******************************************************************************
 *
 *		Windowing
 *
 *******************************************************************************/

bool Renderer::OpenWindow()
{
	Log("Opening Window.");

	// Setup the window settings
	/*if (game->systemInfo->glVersion
	 && (game->systemInfo->glVersion.major < version.major
	 || game->systemInfo->glVersion.minor < version.minor))
	 {
	 version.major = game->systemInfo->glVersion.major;
	 version.minor = game->systemInfo->glVersion.minor;
	 Log("Hardware has limited the OpenGL Version to ", version.major, ".", version.minor);
	 }*/

	Log("OpenGL Version: ", version.major, ".", version.minor);

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, fsaa);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, version.major);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, version.minor);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Fullscreen if needed
	int ret;
	if(fullscreen)
	{
		ret = glfwOpenWindow(resolution().x, resolution().y, 0, 0, 0, 0, 32, 0, GLFW_FULLSCREEN);
	}
	ret = glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW);

	if(ret == GL_FALSE)
	{
		Log("Window Open Failed.");
		return false;
	}

	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		Log("GLEW init failed.");
		return false;
	}

	// Set the window title to the given GAME_NAME.
	glfwSetWindowTitle(((string)game->settings->GetValue("windowtitle")).c_str());

	int minor, major;
	glfwGetGLVersion(&major, &minor, nullptr);

	Log("Context opened in ", major, ".", minor );

	glfwSetWindowCloseCallback(&Renderer::WindowClosed);
	return true;
}

int Renderer::WindowClosed()
{
	Log("Window closed");
	game->Exit();
	return 1;
}

/*******************************************************************************
 *
 *		General Mesh Rendering
 *
 *******************************************************************************/

void Renderer::IndexArray(vector<Vertex>& arr, vector<uint32_t>& idx)
{
	bool added;
	for(uint i = 0; i < arr.size(); ++i)
	{
		added = false;
		for(uint j = 0; j < idx.size(); ++j)
		{
			if(idx.size() == 0)
			{
				break;
			}
			if(arr[idx[j]].vert == arr[i].vert)
			{
				idx.push_back(idx[j]);
				added = true;
				break;
			}
		}

		if(!added)
		{
			idx.push_back(i);
		}
	}

}

void Renderer::AddMesh(Mesh* mesh)
{
	if(mesh->material.shader.name != "")
	{
		mesh->material.shader = LoadShader(mesh->material.shader.name);
		mesh->matrixId = glGetUniformLocation(mesh->material.shader.id, "MVP");
//		game->scene->mainCamera->viewId = glGetUniformLocation( mesh->material.shader.id, "V" );
//		meshr->modelMatrixId = glGetUniformLocation( mesh->material.shader.id, "M" );
		mesh->material.texture.id = glGetUniformLocation(mesh->material.shader.id, "textureSampler");
	}

	glGenBuffers(1, &mesh->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);   // Bind the buffer (vertex array data)
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertices.size(), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mesh->vertices.size(), &mesh->vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	IndexArray(mesh->vertices, mesh->indices);

	glGenBuffers(1, &mesh->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh->indices.size(), &mesh->indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//	glGenVertexArrays( 1, &mesh->vertexArray );

	//	meshBuffer.push_back(meshr);
}

void Renderer::RemoveMesh(Mesh* mesh)
{
	glDeleteBuffers(1, &mesh->vertexBuffer);
	glDeleteBuffers(1, &mesh->indexBuffer);
	glDeleteTextures(1, &mesh->material.texture.id);
//	glDeleteVertexArrays( 1, &mesh->vertexArray );
}

void Renderer::RenderMesh(Mesh* mesh)
{
	if(mesh->vertexBuffer == 0)
	{
		AddMesh(mesh);
	}

	//glPushMatrix();
	if(mesh->material.shader.id)
	{
		glUseProgram(mesh->material.shader.id);
	}

//	glBindVertexArray( mesh->vertexArray );

//	mat4 mvp = game->scene->mainCamera->projection * game->scene->mainCamera->view * meshr->transform->matrix;
//	glUniformMatrix4fv( meshr->matrixId, 1, GL_FALSE, glm::value_ptr( mvp ) );

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	// glVertexAttribPointer ( index, size, type, normalize, stride, offset )
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 12, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 8, (void*)12);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 12, (void*)20);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);

	glDrawElements(GL_TRIANGLES,	// mode
				   mesh->indices.size(),	// count
				   GL_UNSIGNED_INT,	// type
				   (void*)0		// element array buffer offset
				  );

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

//	glBindVertexArray( 0 );
	glUseProgram(0);
}

/*******************************************************************************
 *
 *		Particle System Rendering
 *
 *******************************************************************************/

void Renderer::AddParticles(ParticleSystem* system)
{
	if(system->material.shader.name != "" && system->material.shader.id == 0)
	{
		system->material.shader = LoadShader(system->material.shader.name);
//		sys->matrixId = glGetUniformLocation( sys->material.shader.id, "MVP" );
//		camera->viewId = glGetUniformLocation( sys->material.shader.id, "V" );
//		sys->modelMatrixId = glGetUniformLocation( sys->material.shader.id, "M" );
		system->material.texture.id = glGetUniformLocation(system->material.shader.id, "textureSampler");
	}

	if(system->vertexBuffer == 0)
	{
		glGenBuffers(1, &system->vertexBuffer);
	}
	glBindBuffer(GL_ARRAY_BUFFER, system->vertexBuffer);   // Bind the buffer (vertex array data)
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * system->vertices.size(), 0, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * system->vertices.size(), &system->vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	IndexArray(system->vertices, system->indices);

	if(system->indexBuffer == 0)
	{
		glGenBuffers(1, &system->indexBuffer);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, system->indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * system->indices.size(), 0, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uint32_t) * system->indices.size(), &system->indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//	glGenVertexArrays( 1, &sys->vertexArray );
}

void Renderer::RenderParticles(ParticleSystem* system)
{
	// Shader usage
	glUseProgram(system->material.shader.id);

	// Matrix transformation
	mat4 mvp;
//	glUniformMatrix4fv(sys->matrixId, 1, GL_FALSE, value_ptr(mvp));

	// Binds buffer of vertex attributes, eg. Position, UV, Normal
	glBindBuffer(GL_ARRAY_BUFFER, system->vertexBuffer);

	// glVertexAttribPointer ( index, size, type, normalize, stride, offset )
	glEnableVertexAttribArray(0);   // Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 12, (void*)0);

	glEnableVertexAttribArray(1);   // UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 8, (void*)12);

	glEnableVertexAttribArray(2);   // Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 12, (void*)20);

	// Binds indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, system->indexBuffer);

	// Draws the particles
	glDrawElements(GL_TRIANGLES, system->indices.size(), GL_UNSIGNED_INT, (void*)0);

	// Disables all bound buffers
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

//	glBindVertexArray( 0 );
	glUseProgram(0);
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
	if(sprite->material.shader.name == "" || sprite->material.texture.name == "")
	{
		return;
	}

	for(auto & shader : shaders)
	{
		if(sprite->material.shader.name == shader.name)
		{
			Log("Renderer::AddSprite: About to load shader (already compiled)");
			sprite->material.shader = shader;
		}
	}
	if(sprite->material.shader.id == 0)
	{
		Log("Renderer::AddSprite: About to compile and load shader");
		sprite->material.shader = LoadShader("../../assets/shaders/" + sprite->material.shader.name);
		Log("Renderer::AddSprite: Shader compiled and loaded");
		if(sprite->material.shader.id != 0)
		{
			shaders.push_back(sprite->material.shader);
		}
		else
		{
			return;
		}
	}
	if(sprite->material.shader.id != 0)
	{
		Log("Renderer::AddSprite: About to register shader uniforms");
		sprite->matrixId = glGetUniformLocation(sprite->material.shader.id, "MVP");
		//		game->scene->mainCamera->viewId = glGetUniformLocation( mesh->material.shader.id, "V" );
		//		meshr->modelMatrixId = glGetUniformLocation( mesh->material.shader.id, "M" );
		sprite->material.texture.uniformId = glGetUniformLocation(sprite->material.shader.id, "textureSampler");
	}

	Log("Renderer::AddSprite: About to load texture");
	sprite->material.texture = LoadTexture(sprite->material.texture.name, GL_TEXTURE_2D);

	sprite->vertices.clear();
	sprite->vertices.push_back(Vertex(vec3(sprite->material.texture.width, sprite->material.texture.height, 0), vec2(1,1)));
	sprite->vertices.push_back(Vertex(vec3(sprite->material.texture.width,0,0), vec2(1,0)));
	sprite->vertices.push_back(Vertex(vec3(0,0,0), vec2(0,0)));
	sprite->vertices.push_back(Vertex(vec3(0,sprite->material.texture.height,0), vec2(0,1)));

	glGenBuffers(1, &sprite->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vertexBuffer);   // Bind the buffer (vertex array data)
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * sprite->vertices.size(), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * sprite->vertices.size(), &sprite->vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Log("Renderer: Bound sprite vertices");

	IndexArray(sprite->vertices, sprite->indices);

	Log("Renderer: Indexed sprite indices");

	/*
	glGenBuffers(1, &sprite->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * sprite->indices.size(), &sprite->indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
	glGenVertexArrays(1, &sprite->indexBuffer);

	Log("Renderer: Bound sprite indices");
}

void Renderer::RenderSprite(Sprite* sprite)
{
	if(sprite->vertexBuffer == 0)
	{
		AddSprite(sprite);
	}

	if(sprite->material.shader.id == 0 || sprite->material.texture.id == 0)
	{
		return;
	}

//glPushMatrix();
	glUseProgram(sprite->material.shader.id);


	glBindVertexArray( sprite->indexBuffer );

//	glBindVertexArray( mesh->vertexArray );

	mat4 mvp = game->scene->mainCamera->projection * game->scene->mainCamera->view * sprite->transform->matrix();
	glUniformMatrix4fv(sprite->matrixId, 1, GL_FALSE, &mvp[0][0]);

	//Log("Renderer: Sprite mvp loaded");

	// Texture setup
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->material.texture.id);
	glUniform1i(sprite->material.texture.uniformId, 0);

	//Log("Renderer: Sprite texture bound");

	glBindBuffer(GL_ARRAY_BUFFER, sprite->vertexBuffer);

// glVertexAttribPointer ( index, size, type, normalize, stride, offset )
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 12, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 8, (void*)12);

	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) - 12, (void*)20);

	//Log("Renderer: Sprite attributes sent");

	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->indexBuffer);

	glDrawElements(GL_TRIANGLES,	// mode
				   sprite->indices.size(),	// count
				   GL_UNSIGNED_INT,	// type
				   (void*)0		// element array buffer offset
				  );
	*/
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );

//	glBindVertexArray( 0 );
	glUseProgram(0);
}

void Renderer::RemoveSprite(Sprite* sprite)
{
	glDeleteBuffers(1, &sprite->vertexBuffer);
	glDeleteBuffers(1, &sprite->indexBuffer);
	glDeleteTextures(1, &sprite->material.texture.id);
//	glDeleteVertexArrays( 1, &mesh->vertexArray );
}

/*******************************************************************************
 *
 *		Gets/Sets/Shaders
 *
 *******************************************************************************/

ivec2 Renderer::GetResolution()
{
	return _resolution;
}

void Renderer::SetResolution(const ivec2& res)
{
	_resolution = res;
	OpenWindow();
}

void Renderer::SetVersion(const byte major, const byte minor)
{
	version.major = major;
	version.minor = minor;
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
