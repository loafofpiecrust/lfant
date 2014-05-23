/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

// Internal
#include <lfant/ptr.h>
#include <lfant/Subsystem.h>

// External
#include <deque>

/*
namespace CEGUI
{
class OpenGL3Renderer;
class Window;
class WindowManager;
class System;
class GUIContext;
class EventArgs;
class InputAggregator;
}
*/



/*
namespace gameswf
{
struct player;
struct root;
struct render_handler;
}
*/

namespace lfant
{

namespace gui
{
class Renderer;
class System;
class FileSystem;
}

class UserInterface : public Subsystem
{
public:
	class Movie
	{
	public:
	//	Movie(string name, gameswf::root* swf);
		~Movie();

		void Play();
		void Pause();

		string name = "";
	//	ptr<gameswf::root> swf; //= nullptr;
		bool active = true;
	};

	UserInterface(Game* game);
	virtual ~UserInterface();

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	virtual void* GetRoot() = 0;

//	virtual void CreateWindow(Properties* prop, CEGUI::Window *parent = nullptr);
//	virtual void RemoveWindow(string fileName);

	virtual void OnKey(uint16 key, int mode, int mods) = 0;
	virtual void OnChar(char key) = 0;
	virtual void OnMouseButton(uint16 btn, int mode, int mods) = 0;
	virtual void OnMouseMove(ivec2 pos) = 0;
	virtual void OnWindowResize(int width, int height) = 0;

//	Movie* LoadMovie(string name, string path);
//	Movie* GetMovie(string name);

#if !LFANT_GLES

//	bool OnClickButton(const CEGUI::EventArgs &evt);
//	bool OnCloseWindow(const CEGUI::EventArgs &evt);

//	CEGUI::Window* rootWindow = nullptr;

protected:
/*
	CEGUI::OpenGL3Renderer* renderer = nullptr;
	CEGUI::WindowManager* windowManager = nullptr;
//	std::deque<CEGUI::Window*> windows;
	CEGUI::System* system = nullptr;
	CEGUI::GUIContext* context = nullptr;
	CEGUI::InputAggregator* input = nullptr;
*/
#endif

	/*
	ptr<gameswf::player> player;
	ptr<gameswf::render_handler> renderer;
	deque< ptr<Movie> > movies;
	ptr<Movie> root;
	*/

	string rootResFolder = "gui";
};

}
