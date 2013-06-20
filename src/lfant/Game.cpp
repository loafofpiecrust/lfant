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

#include "Game.h"
#include "Scene.h"

namespace lfant
{

Game* game;

Game::Game() :
	console {new Console},
	fileSystem {new FileSystem},
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
	LoadFile("settings/game.cfg");

	Log("Initing filesystem");
	fileSystem->Init();
//	settings->Init();
	Log("Initing systeminfo");
//	systemInfo->Init();
	Log("Initing time");
	time->Init();
	Log("Initing physics");
	physics->Init();
	renderer->Init();
	input->Init();
//	audio->Init();
//	network->Init();
	scriptSystem->Init();
//	userInterface->Init();
	scene->Init();

	if(defaultScene != "") scene->LoadFile(defaultScene);

	Log("Window callback set.");

	Log("Subsystems initialized");
}

void Game::Update()
{
	renderer->PreUpdate();
	time->Update();
	scene->Update();
	physics->Update();
	input->Update();
//	userInterface->Update();
//	network->Update();
	renderer->Update();
}


/*******************************************************************************
*
*		Loading/Saving
*
*******************************************************************************/

void Game::Load(Properties* prop)
{
	prop->Get("orgName", orgName);
	prop->Get("gameName", gameName);
	prop->Get("defaultScene", defaultScene);
	Log("Game loading, default scene is '", defaultScene, "'.");
}

void Game::Save(Properties* prop)
{
	prop->Set("orgName", orgName);
	prop->Set("gameName", gameName);
	prop->Set("defaultScene", defaultScene);
}

/*******************************************************************************
*
*		General Game Functions
*
*******************************************************************************/

void Game::Destroy()
{
	fileSystem->Destroy();
	systemInfo->Destroy();
	time->Destroy();
	physics->Destroy();
	renderer->Destroy();
	userInterface->Destroy();
	scene->Destroy();
	Log("Scene destroyed.");
	input->Destroy();
	audio->Destroy();
	network->Destroy();
	console->Destroy();
}

void Game::Exit()
{
	destroy = true;
}

bool Game::IsExited()
{
	return destroy;
}

}
