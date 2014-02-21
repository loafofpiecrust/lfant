/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-15 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Game.h>

// External

// Internal
#include <lfant/Renderer.h>
#include <lfant/Input.h>
#include <lfant/Time.h>
#include <lfant/Physics.h>
#include <lfant/Audio.h>
#include <lfant/Scene.h>
#include <lfant/FileSystem.h>
#include <lfant/SystemInfo.h>
#include <lfant/Console.h>
#include <lfant/UserInterface.h>
#include <lfant/Network.h>
#include <lfant/ScriptSystem.h>

#if !ANDROID
#include <lfant/OpenCL.h>
#endif

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
#if !ANDROID
	,openCL {new OpenCL}
#endif
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
	
	Log("Initing time");
	time->Init();
	
	LoadFile("settings/game.prop");

	Log("Initing filesystem");
	fileSystem->Init();
//	settings->Init();
	Log("Initing systeminfo");
//	systemInfo->Init();
	Log("Initing physics");
	physics->Init();
	renderer->Init();
#if !ANDROID
	openCL->Init();
#endif
	input->Init();
//	audio->Init();
	network->Init();
	scriptSystem->Init();
	if(standAlone) userInterface->Init();
	scene->Init();

	Log("Window callback set.");

	Log("Subsystems initialized");
}

void Game::Update()
{
	if(standAlone) renderer->PreUpdate();

	time->Update();
	scene->Update();
	physics->Update();
	input->Update();
	network->Update();

	if(standAlone)
	{
		scene->Render();
		renderer->Update();
		userInterface->Update();
	}
}


/*******************************************************************************
*
*		Loading/Saving
*
*******************************************************************************/

void Game::Load(Properties* prop)
{
	Subsystem::Load(prop);

	prop->Get("orgName", orgName);
	prop->Get("gameName", gameName);
	prop->Get("defaultScene", defaultScene);
	Log("Game name: "+gameName);
	Log("Game loading, default scene is '"+defaultScene+"'.");
}

void Game::Save(Properties* prop) const
{
	Subsystem::Save(prop);

	prop->Set("orgName", orgName);
	prop->Set("gameName", gameName);
	prop->Set("defaultScene", defaultScene);
}

void Game::Bind()
{
//	Script::Class<Game, Object> inst;
}

/*******************************************************************************
*
*		General Game Functions
*
*******************************************************************************/

void Game::Destroy()
{
	Deinit();
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
	delete this;
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
