/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// External
#include <thread>
#include <functional>

// Internal
#include <lfant/ptr.h>

#include <lfant/Subsystem.h>
#include <lfant/Console.h>
#include <lfant/util/Path.h>

namespace lfant {

class Renderer;
class Input;
class Time;
class Physics;
class Audio;
class Scene;
class FileSystem;
//class Console;
class SystemInfo;
class UserInterface;
class Network;
class ScriptSystem;
class OpenCL;
class Window;

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**	The main Game class controls the starting and updating of the game.
 *	Controls the creation of a scene and the base control of the main game
 *	subsystems. This includes networking, input, settings, physics, and
 *	scripts. It doesn't directly control these, but it holds pointers to
 *	the classes that do.
 */
class Game : public Object
{
public:
	Game();
	Game(const Game& other);
	virtual ~Game();

	virtual void Serialize(Properties *prop);

	static void ScriptBind();

	virtual Game* GetGame() const;

	/**
	 *	This function is called right when the game is launched.
	 */
	virtual void Init();

	/**
	 *	Called every frame to update the game and its subsystems
	 */
	virtual void Update();

	/**
	 *	Destroys the game instance.
	 */
	virtual void Destroy();

	/**
	 *	Schedules the game to be exited at the end of this frame.
	 */
	virtual void Exit();

	/**
	 *	Adds the given function to a new or existing thread.
	 *	@param func The function to execute.
	 */
	std::thread& AddThread(std::function<void()> th);

	bool IsExited();

	template<typename T = const char*>
	void Log(T msg)
	{
		console->Print(msg);
	}

	template<typename T, typename P, typename ... A>
	void Log(T msg, P msg2, A... args)
	{
		console->LinePrint(msg);
		Log(msg2, args...);
	}


	boost::filesystem::path GetUserPath(string name) const;
	boost::filesystem::path GetAssetPath(string name) const;

	std::deque<boost::filesystem::path> GetGameFiles(string dir) const;

	string GetProgramDir();


	string orgName = "lazyfox";
	string gameName = "lfant";
	string defaultScene = "";

	string programFolder = "./";
	string gameFolder = "../..";
	string userFolder;

	// Subsystems
	ptr<Console> console;
	ptr<Time> time;
	ptr<SystemInfo> systemInfo;
	ptr<Window> window;
	ptr<Renderer> renderer;
	ptr<Input> input;

	ptr<Physics> physics;
	ptr<Network> network;
	ptr<ScriptSystem> scriptSystem;
	ptr<UserInterface> userInterface;
	ptr<Audio> audio;
#if !ANDROID
	ptr<OpenCL> openCL;
#endif
	ptr<Scene> scene;

	bool standAlone = true;

private:
	bool destroy = false;
	std::deque<std::thread> threads;

};

//extern Game* game;


/** @} */
/** @} */
}

struct android_app;
namespace lfant {
namespace android {
	void Launch(android_app* state);
}
}
