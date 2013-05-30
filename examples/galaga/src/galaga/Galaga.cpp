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
#include <lfant/UserInterface.h>
#include <lfant/Network.h>
#include <lfant/ChatClient.h>
#include <lfant/Mesh.h>
#include <lfant/Entity.h>
#include <lfant/Transform.h>
#include <lfant/Sprite.h>

#include <galaga/Player.h>

#include <lfant/network/Server.h>
#include <lfant/network/Client.h>

namespace lfant
{

extern "C" void Launch()
{
	printf("Launched\n");
	game = new Galaga();
	game->standAlone = true;
	game->Init();
	Log("Game initialised");
	game->Update();
	Log("Game ending");
	delete game;
}

Galaga::Galaga()
{
}

void Galaga::OnHost()
{
	Log("Galaga::OnHost/OnConnect: Touch.");
	//	client->SendData("12345678");
	//	client->SendFile("scenes/init.scene", "scenes/new.scene");
}

void Galaga::Init()
{
	Game::Init();

	scene->LoadFile("scenes/main.scene");
	scene->SaveFile("scenes/init.scene");
	Log("Finished loading scene");

	userInterface->LoadFile("gui/MainMenu.gui");

	//	server = network->AddConnection<network::Server>();
	/*
	Log("Setting up client...");
	client = new network::Client;
	client->Host(22222);

	Log("Setting up client2...");
	client2 = new network::Client;
	client2->Connect("127.0.0.1", 22222);

	client2->GetDataAsync(256);
	*/
	ent = scene->Spawn();
//	ent2 = scene->Spawn();
	client = ent->AddComponent<ChatClient>();
//	client2 = ent2->AddComponent<ChatClient>();

	client->Connect("74.177.199.145");
//	client2->Connect();

	ConnectEvent(SENDER(client, Host), RECEIVER(this, OnHost));
	ConnectEvent(SENDER(client, Connect), RECEIVER(this, OnHost));

	//	client->SendData("12345678");

	//	client->GetDataAsync(8);


}

void Galaga::Update()
{
	while(!IsExited())
	{
		Game::Update();
	}
	Destroy();
}

void Galaga::Destroy()
{
	scene->SaveFile("scenes/final.scene");

//	client->SendMessage("Hello my friend.");
//	client2->SendMessage("Hello yourself.");
	/*
	for(uint i = 0; i < 10; ++i)
	{
		client->Update();
		client2->Update();
	}
	*/

	Game::Destroy();
}

void Galaga::AddMesh(string name)
{
	Entity* ent = scene->Spawn(name, nullptr);
	ent->GetComponent<Transform>()->SetPosition(vec3(1, 1, 1));
	Mesh* mesh = ent->AddComponent<Mesh>();
	mesh->LoadFile("suzanne.obj");
	mesh->material->texture->LoadFile("player.png");
	//	mesh->material->shader->path = "shaders/Diffuse";
}

}
