/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-01-10 by Taylor Snead
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

#include "Galaga.hpp"

// External

// Internal
#include "Renderer.hpp"
#include "Time.hpp"
#include "Console.hpp"
#include "SystemInfo.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Input.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"
#include "TypeInfo.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Settings.hpp"
#include "FileSystem.hpp"
#include "UserInterface.hpp"
#include "Network.hpp"
#include "ChatClient.hpp"

namespace lfant
{

extern "C" void Launch()
{
	game = new Galaga();
	game->standAlone = true;
	game->Init();
	// game->destroy gets set true when game->Exit() is called. Prevents mid-frame destruction, for safety.
	game->destroy = false;
	Log("Game initialised");
	while (!game->destroy)
	{
		game->Update();
	}
	Log("About to destroy game");
	game->Destroy();
	Log("Game ending");
	delete game;
}

Galaga::Galaga()
{
}

void Galaga::Init()
{
	Engine::Init();

	//Range<int> ver = game->settings->GetValue("gl.version");
	//renderer->version = ver;

	//ivec2 res = game->settings->GetValue("window.resolution").i2();
	//renderer->SetResolution(res);
	//ivec2 pos = game->settings->GetValue("window.position").i2();
	//renderer->SetPosition(pos);

	// Look at Player.cpp
	input->AddAxis("Horizontal", Key::D, Key::A);
	input->AddAxis("Vertical", Key::W, Key::S);
	input->AddAxis("VRotation", Key::Up, Key::Down);
	input->AddAxis("HRotation", Key::Right, Key::Left);
	input->AddAxis("ShowLoc", Key::E);
	input->AddAxis("ShowRot", Key::R);
	input->AddAxis("ShowFPS", Key::Q);
	input->AddAxis("Fire", Key::Space);
	input->AddAxis("TesterSetVar", Key::X);
	input->AddAxis("TesterGetVar", Key::Z);
	input->AddAxis("TesterHelpMe", Key::H);
	input->AddAxis("Exit", Key::Esc);

	// Just tests SystemInfo (very minimal on Linux, nothing on OSX, some on Windows)
	Log("Computer name: " + systemInfo->computerName);
	Log("Username: " + systemInfo->username);
	Log("Running OS: " + systemInfo->OS);
	Log("CPU: " + systemInfo->cpu.name + " with ", systemInfo->cpu.cores, " cores");

	// Fairly self-explanitory. Check above input axes for keys to press. Will implement better with GUI.
	console->RegisterVar("Tester", 5.0f, "This is my variable!");

	/*
	player = scene->Spawn("Snead", nullptr, vec3(0, 0, 0));
	player->AddComponent<Player>();
	scene->mainCamera = player->AddComponent<Camera>();
	scene->mainCamera->projectionMode = Camera::PM_PERSPECTIVE;


	Entity* ent = scene->Spawn("TestMesh", nullptr);
	Mesh* mesh = ent->AddComponent<Mesh>();
	mesh->LoadFile("suzanne.obj");
	mesh->material.texture.LoadFile("player.png");
	mesh->material.shader.name = "Diffuse";
	*/

	Entity* ent = scene->Spawn("ChatClientObj", nullptr);
	ent->AddComponent<ChatClient>();

	//Log("Mesh parent: "+ent->parent->name);

	/*
	Entity* ent = Entity::Spawn("TestSprite", nullptr);
	Sprite* sprite = ent->AddComponent<Sprite>();
	//mesh->LoadFile("suzanne.obj");
	sprite->material.texture.LoadFile("player.png");
	sprite->material.shader.name = "Diffuse";
	*/

	/*
	for(uint i = 0; i < game->settings->GetValue("galaga.meshcount").i(); ++i)
	{
		AddMesh("TestMesh"+lexical_cast<string>(i));
		Log("Added mesh ", i);
	}
	*/
}

void Galaga::Update()
{
	PreUpdate();
	renderer->PreUpdate();
	time->Update();
	physics->Update();
	scene->Update();
	input->Update();
//	userInterface->Update();
	network->Update();
	renderer->Update();
	PostUpdate();
}

void Galaga::Destroy()
{
	Engine::Destroy();
}

void Galaga::AddMesh(string name)
{
	Entity* ent = scene->Spawn(name, nullptr, vec3(1,1,1));
	Mesh* mesh = ent->AddComponent<Mesh>();
	mesh->LoadFile("suzanne.obj");
	mesh->material.texture.LoadFile("player.png");
	mesh->material.shader.name = "shaders/Diffuse";
}

}
