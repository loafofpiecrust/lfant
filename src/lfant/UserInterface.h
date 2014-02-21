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

namespace CEGUI
{
class OpenGL3Renderer;
class Window;
class WindowManager;
class System;
class GUIContext;
class EventArgs;
}

/*
   namespace Rocket
   {
   namespace Core
   {

   class Context;
   class ElementDocument;

   }
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

	UserInterface();
	virtual ~UserInterface();

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	virtual void Load(Properties *prop);
	virtual void Save(Properties *prop) const;

	virtual void CreateWindow(Properties* prop, CEGUI::Window *parent = nullptr);
	virtual void RemoveWindow(string fileName);

	virtual void OnKey(uint16_t key, int mode);
	virtual void OnChar(char key);
	virtual void OnMouseButton(uint16 btn, int mode);
	virtual void OnMouseMove(vec2 pos);
	virtual void OnWindowResize(int32_t width, int32_t height);


//	Movie* LoadMovie(string name, string path);
//	Movie* GetMovie(string name);

#if !LFANT_GLES

	bool OnClickButton(const CEGUI::EventArgs &evt);
	bool OnCloseWindow(const CEGUI::EventArgs &evt);

	CEGUI::Window* rootWindow = nullptr;

protected:

	CEGUI::OpenGL3Renderer* renderer = nullptr;
	CEGUI::WindowManager* windowManager = nullptr;
//	std::deque<CEGUI::Window*> windows;
	CEGUI::System* system = nullptr;
	CEGUI::GUIContext* context = nullptr;
#endif

	/*
	   ptr<Rocket::Core::Context> context;
	   ptr<gui::Renderer> renderer;
	   ptr<gui::System> system;
	   ptr<gui::FileSystem> fileSystem;
	   deque< ptr<Rocket::Core::ElementDocument> > documents;
	 */

	/*
	ptr<gameswf::player> player;
	ptr<gameswf::render_handler> renderer;
	deque< ptr<Movie> > movies;
	ptr<Movie> root;
	*/

	bool resized = false;
	ivec2 size;

	string rootResFolder = "gui";
};

}
