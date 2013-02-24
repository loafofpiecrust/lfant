/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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
#pragma once
#include "stdafx.hpp"

// External

// Internal
//
#include "Subsystem.hpp"

namespace sfs
{

class Renderer;
class Input;
class Time;
class Physics;
class Settings;
class Audio;
class Scene;
class FileSystem;
class Console;
class SystemInfo;
class UserInterface;
class Network;

/** @addtogroup Engine
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
class Engine : public Subsystem
{
public:
	Engine();
	virtual ~Engine();

	/**	This function is called right when the game is launched.
	 *	@todo
	 *		Make virtual when this needs to be overwritten/extended.
	 */
	virtual void Init();

	/**	Called every frame to update the game and its subsystems
	 *	@todo
	 *		Have this call the update of all other objects of the factories.
	 *		Make this virtual later when/if it needs to be overwritten.
	 */
	virtual void Update();
	virtual void PreUpdate()
	{
	}
	virtual void PostUpdate()
	{
	}

	/**
	 * Destroys the game instance. Never call manually.
	 */
	virtual void Destroy();

	/**
	 *	Schedules the game to be exited at the end of this frame.
	 */
	virtual void Exit();

	/**	Loads a scene. Automatically appends the given scene with an
	 *	extension. That extension is assigned via the ResourceManager,
	 *	along with file paths.
	 *	@param scene The name of the scene to load, without the file extension.
	 */
	void LoadScene(string scene);

	/** Adds the given function to a new or existing thread.
	 *	@param func The function to execute.
	 */
	void AddThread(boost::function<void()> func);

	// Subsystems
	unique_ptr<Console> console;
	unique_ptr<FileSystem> fileSystem;
	unique_ptr<Settings> settings;
	unique_ptr<SystemInfo> systemInfo;
	unique_ptr<Time> time;
	unique_ptr<Physics> physics;
	unique_ptr<Renderer> renderer;
	unique_ptr<UserInterface> userInterface;
	unique_ptr<Scene> scene;
	unique_ptr<Input> input;
	unique_ptr<Audio> audio;
	unique_ptr<Network> network;
	//boost::scoped_ptr<AISystem>			aiSystem;
	//boost::scoped_ptr<ScriptSystem>		scriptSystem;
	//boost::scoped_ptr<FGSystem>			flowgraph;

	string gameName = "ShadowFox Engine";

	bool standAlone;
	bool destroy = false;

protected:

private:
};

extern Engine* game;

/** @} */
/** @} */
}
