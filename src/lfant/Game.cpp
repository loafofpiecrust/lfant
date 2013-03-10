/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-07-15 by Taylor Snead
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


#include <lfant/Game.h>

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
#include <lfant/ScriptSystem.h>

namespace lfant
{

Game* game;

extern "C" void Launch()
{
	game = new Game();
	game->standAlone = true;
	game->Init();
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

Game::Game() :
	console {new Console},
	fileSystem {new FileSystem},
	settings {new Settings},
	systemInfo {new SystemInfo},
	time {new Time},
	physics {new Physics},
	renderer {new Renderer},
	userInterface {new UserInterface},
	scene (new Scene),
	input {new Input},
	audio {new Audio},
	network {new Network},
	scriptSystem {new ScriptSystem}
{
}

Game::~Game()
{
}

/*******************************************************************************
 *
 *		Generic Subsystem Functions
 *
 *******************************************************************************/

void Game::Init()
{
	console->Init();
	fileSystem->Init();
	settings->Init();
	systemInfo->Init();
	time->Init();
	physics->Init();
	renderer->Init();
	userInterface->Init();
	scene->Init();
	input->Init();
//	audio->Init();
	network->Init();
	scriptSystem->Init();

	Log("Window callback set.");

	Log("Subsystems initialized");
}

void Game::Update()
{
	PreUpdate();
	time->Update();
	physics->Update();
	scene->Update();
	input->Update();
	renderer->Update();
//	userInterface->Update();
	network->Update();
	PostUpdate();
}

/*******************************************************************************
 *
 *		General Game Functions
 *
 *******************************************************************************/

void Game::LoadScene(string scene)
{
}

void Game::Destroy()
{
	console->Destroy();
	fileSystem->Destroy();
	settings->Destroy();
	systemInfo->Destroy();
	time->Destroy();
	physics->Destroy();
	renderer->Destroy();
	userInterface->Destroy();
	scene->Destroy();
	input->Destroy();
	audio->Destroy();
	network->Destroy();
}

void Game::Exit()
{
	destroy = true;
}

}
