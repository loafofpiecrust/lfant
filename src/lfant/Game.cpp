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
#include <lfant/Window.h>

#include "lfant/gui/gwen/UserInterface.h"

#if !ANDROID
#include <lfant/OpenCL.h>
#endif

#include "Game.h"
#include "Scene.h"

// External
#include <thread>
#include <functional>

#if WINDOWS
#include <direct.h>
#endif

namespace lfant
{

string GetProgramDirRaw()
#if LINUX || ANDROID
{
	char tmp[128];
	uint len = readlink("/proc/self/exe", tmp, sizeof(tmp)-1);
	if(len != -1)
	{
		tmp[len] = '\0';
	}
	boost::filesystem::path result {tmp};

	return result.remove_filename().string();
}
#elif WINDOWS
{
	HMODULE hmod = GetModuleHandle(0);
	char file[128];
	GetModuleFileName(hmod, file, 128);

	boost::filesystem::path p(file);
	p.remove_filename();
	return p.string();
}
#elif MACOSX

#endif

Game* game;

Game::Game() :
	programFolder(GetProgramDirRaw()),
	gameFolder(boost::filesystem::canonical(boost::filesystem::path(programFolder+"/../../..")).string())

	,console {new Console(this)}
	,time {new Time(this)}
//	,fileSystem {new FileSystem(this)}
	,systemInfo {new SystemInfo(this)}
	,window {new Window(this)}
	,renderer {new Renderer(this)}
	,input {new Input(this)}

	,physics {new Physics(this)}
	,network {new Network(this)}
	,scriptSystem {new ScriptSystem(this)}
	,userInterface {new gui::gwen::UserInterface(this)}
	,audio {new Audio(this)}
#if !ANDROID
	,openCL {new OpenCL(this)}
#endif
	,scene {new Scene(this)}
{
}

Game::Game(const Game& other)
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

string Game::GetProgramDir()
{
	return programFolder;
}

void Game::Init()
{
	console->Init();

	Log("Initing time");
	time->Init();

	LoadFile("settings/game.prop");
#if WINDOWS
	userFolder = string(getenv("USERPROFILE")) + "/Documents/My Games/" + orgName + "/" + gameName;
#elif UNIX
	userFolder = string(getenv("HOME")) + "/Documents/My Games/" + orgName + "/" + gameName;
#elif ANDROID

#endif

#if WINDOWS
	_chdir((gameFolder+"/assets/").c_str());
#else
	chdir((gameFolder+"/assets/").c_str());
#endif

//	settings->Init();
	Log("Initing systeminfo");
//	systemInfo->Init();
	Log("Initing physics");
	if(standAlone) window->Init();
	renderer->Init();
	input->Init();
	physics->Init();

	network->Init();
//	network->Init();
	scriptSystem->Init();
	if(standAlone) userInterface->Init();
//	audio->Init();
#if !ANDROID
//	openCL->Init();
#endif

	// Finally, init the scene
	scene->Init();

//	AddThread(std::bind(&Network::RunLoop, network.get()));

	GetGame()->Log("Game initialized");
}

void Game::Update()
{
//	network->mutex.lock();

	time->Update();
	scene->Update();
	physics->Update();
	input->Update();
	if(standAlone)
	{
		window->Update();
	}
//	network->Update();

	if(standAlone)
	{
		renderer->Update();
	}
//	network->mutex.unlock();
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
	GetGame()->Log("Game name: "+gameName);
	GetGame()->Log("Game loading, default scene is '"+defaultScene+"'.");
}

void Game::Save(Properties* prop) const
{
	prop->Set("orgName", orgName);
	prop->Set("gameName", gameName);
	prop->Set("defaultScene", defaultScene);
}

void Game::ScriptBind()
{
	Script::Class<Game, Object> inst;
	/// @todo Figure out how to bind and access smart pointers

	inst.Bind();
}

Game* Game::GetGame() const
{
	return const_cast<Game*>(this);
}

/*******************************************************************************
*
*		General Game Functions
*
*******************************************************************************/

void Game::Destroy()
{
	if(!destroy) destroy = true;
//	(*threads.end()).join();

	Deinit();
	scene->Destroy(); scene.reset();
	scriptSystem->Destroy(); scriptSystem.reset();
	GetGame()->Log("Scene destroyed.");
//	openCL->Destroy();
//	audio->Destroy();
	userInterface->Destroy();
	network->Destroy();
	physics->Destroy();
	input->Destroy();
	renderer->Destroy();
	if(standAlone) window->Destroy();
	systemInfo->Destroy();
//	fileSystem->Destroy();
	time->Destroy();
	console->Destroy();
	delete this;
}

void Game::Exit()
{
	destroy = true;
}

std::thread& Game::AddThread(std::function<void()> th)
{
	threads.emplace_back(th);
	return *threads.end();
}

bool Game::IsExited()
{
	return destroy;
}



boost::filesystem::path Game::GetAssetPath(string name) const
{
	boost::filesystem::path result(gameFolder + "/assets/" + name);
	if(!exists(result))
	{
		GetGame()->Log("FileSystem::GetGamePath: File not found '"+result.string()+"'.");
	//	game->Exit();
		return result;
	}
	return canonical(result);
}

boost::filesystem::path Game::GetUserPath(string name) const
{
	boost::filesystem::path result(userFolder + "/" + name);
	if(!exists(result))
	{
		GetGame()->Log("FileSystem::GetUserPath: File not found '"+result.string()+"'.");
	}
	return result;
}

std::deque<boost::filesystem::path> Game::GetGameFiles(string dir) const
{
	std::deque<boost::filesystem::path> result;
	if(boost::filesystem::exists(dir) && boost::filesystem::is_directory(dir))
	{
		copy(boost::filesystem::directory_iterator(dir), boost::filesystem::directory_iterator(), back_inserter(result));
	}
	return result;
}

}
