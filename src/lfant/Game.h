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

// Internal
#include <lfant/ptr.h>

#include <lfant/Subsystem.h>

namespace lfant {

class Renderer;
class Input;
class Time;
class Physics;
class Audio;
class Scene;
class FileSystem;
class Console;
class SystemInfo;
class UserInterface;
class Network;
class ScriptSystem;
class OpenCL;

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
class Game : public Subsystem
{
public:
	Game();
	Game(const Game& other);
	virtual ~Game();

	virtual void Load(Properties *prop);
	virtual void Save(Properties *prop) const;

	static void ScriptBind();

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
	void AddThread(boost::function<void()> func);

	bool IsExited();

	// Subsystems
	ptr<Console> console;
	ptr<FileSystem> fileSystem;
	ptr<SystemInfo> systemInfo;
	ptr<Time> time;
	ptr<Physics> physics;
	ptr<Renderer> renderer;
	ptr<UserInterface> userInterface;
	ptr<Scene> scene;
	ptr<Input> input;
	ptr<Audio> audio;
	ptr<Network> network;
	ptr<ScriptSystem> scriptSystem;

#if !ANDROID
	ptr<OpenCL> openCL;
#endif
	//boost::scoped_ptr<AISystem>			aiSystem;
	//boost::scoped_ptr<FGSystem>			flowgraph;

	bool standAlone = true;

	string orgName = "lazyfox";
	string gameName = "lfant";
	string defaultScene = "";

protected:

private:
	bool destroy = false;
};

extern Game* game;


/** @} */
/** @} */
}

struct android_app;
namespace lfant {
namespace android {
	void Launch(android_app* state);
}
}
