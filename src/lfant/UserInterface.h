/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-02-13 by Taylor Snead
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

namespace gameswf
{
struct player;
struct root;
struct render_handler;
}

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
		Movie(string name, gameswf::root* swf);
		~Movie();

		void Play();
		void Pause();

		string name = "";
		ptr<gameswf::root> swf; //= nullptr;
		bool active = true;
	};

	UserInterface();
	virtual ~UserInterface();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	void Load(Properties *prop);
	void Save(Properties *prop);

	virtual void CreateWindow(Properties* prop, CEGUI::Window *parent = nullptr);
	virtual void RemoveWindow(string fileName);

	virtual void OnKey(uint16 key, int mode);
	virtual void OnChar(char key);
	virtual void OnMouseButton(uint16 btn, int mode);
	virtual void OnMouseMove(int x, int y);
	virtual void OnWindowResize(uint width, uint height);

	bool OnClickButton(const CEGUI::EventArgs &evt);
	bool OnCloseWindow(const CEGUI::EventArgs &evt);

//	Movie* LoadMovie(string name, string path);
//	Movie* GetMovie(string name);

	CEGUI::Window* rootWindow;

protected:

	
	CEGUI::OpenGL3Renderer* renderer;
	CEGUI::WindowManager* windowManager;
	deque<CEGUI::Window*> windows;
	CEGUI::System* system;
	CEGUI::GUIContext* context;
	

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
	uvec2 size;
};

}
