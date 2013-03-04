/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
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

#include <galaga/Galaga.h>

// External

// Internal
#include <lfant/Renderer.h>
#include <lfant/Input.h>
#include <lfant/Time.h>
#include <lfant/Physics.h>
#include <lfant/Settings.h>
#include <lfant/Audio.h>
#include <lfant/Scene.h>
#include <lfant/FileSystem.h>
#include <lfant/Console.h>
#include <lfant/SystemInfo.h>
#include <lfant/Console.h>
#include <lfant/UserInterface.h>
#include <lfant/Network.h>
#include <lfant/ChatClient.h>
#include <lfant/Mesh.h>
#include <lfant/Entity.h>
#include <lfant/Transform.h>

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

	Log("Galaga::Init: Registered test var");

	Entity* ent = nullptr;
	Log("Galaga::Init: Allocated Entity pointer");

	ent = scene->Spawn("Player");

	Log("Galaga::Init: Spawned player");


	ent = scene->Spawn("ChatClientObj", nullptr);
	ent->AddComponent<ChatClient>();
}

void Galaga::Update()
{
	PreUpdate();
	renderer->PreUpdate();
	time->Update();
	physics->Update();
	scene->Update();
	input->Update();
	userInterface->Update();
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
	Entity* ent = scene->Spawn(name, nullptr);
	ent->GetComponent<Transform>()->SetPosition(vec3(1, 1, 1));
	Mesh* mesh = ent->AddComponent<Mesh>();
	mesh->LoadFile("suzanne.obj");
	mesh->material.texture.LoadFile("player.png");
	mesh->material.shader.name = "shaders/Diffuse";
}

}
