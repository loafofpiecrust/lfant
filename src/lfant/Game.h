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
#pragma once

#include <lfant/stdafx.h>

// External

// Internal
#include <lfant/ptr.h>

#include <lfant/Subsystem.h>

namespace lfant
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
class ScriptSystem;

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
	virtual ~Game();

	/**	This function is called right when the game is launched.
	 */
	virtual void Init();

	/**	Called every frame to update the game and its subsystems
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
	ptr<Console> console;
	ptr<FileSystem> fileSystem;
	ptr<Settings> settings;
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
	//boost::scoped_ptr<AISystem>			aiSystem;
	//boost::scoped_ptr<ScriptSystem>		scriptSystem;
	//boost::scoped_ptr<FGSystem>			flowgraph;

	bool standAlone = true;
	bool destroy = false;

protected:

private:
};

extern Game* game;

/** @} */
/** @} */
}
