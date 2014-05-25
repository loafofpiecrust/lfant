/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

// Internal
#include <lfant/UserInterface.h>
#include <lfant/Thread.h>

#include <lfant/Game.h>
#include <lfant/FileSystem.h>
#include <lfant/Console.h>
#include <lfant/Input.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Time.h>
#include <lfant/Thread.h>
#include <lfant/Renderer.h>

// External
#if !LFANT_GLES
//#include <GLFW/glfw3.h>

/*
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/DefaultResourceProvider.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI/XMLParserModules/TinyXML/XMLParser.h>
*/
#include "FrameBuffer.h"

#endif
/*
#include <gameswf/gameswf.h>
#include <base/tu_file.h>
#include <base/tu_types.h>
#include <gameswf/gameswf_types.h>
#include <gameswf/gameswf_player.h>
#include <gameswf/gameswf_root.h>

*/

using namespace std;

namespace lfant {

UserInterface::UserInterface(Game* game) :
	Subsystem(game)
{
}

UserInterface::~UserInterface()
{
}

#if !LFANT_GLES
// gameswf
/*
static tu_file*	file_opener(const char* url)
// Callback function.  This opens files for the gameswf library.
{
	return new tu_file(game->GetAssetPath(url).c_str(), "rb");
}

static void log_callback(bool error, const char* message)
{
	GetGame()->Log(message);
}

void UserInterface::Init()
{
	player = new gameswf::player;

	player->set_separate_thread(false);

	player->set_force_realtime_framerate(30);
	player->set_workdir("../../assets");
	gameswf::set_glyph_provider(gameswf::create_glyph_provider_tu());

	renderer = gameswf::create_render_handler_ogl();
	gameswf::set_render_handler(renderer);
	renderer->open();
	renderer->set_antialiased(true);

	gameswf::register_file_opener_callback(file_opener);
	gameswf::register_log_callback(log_callback);


//	root = new Movie("root", player->get_root());

	Subsystem::Init();
}

void UserInterface::Update()
{
//	static Movie* m = root;
	for(ptr<Movie>& m : movies)
	{
		GetGame()->Log("Player has root? ", player->get_root());
		if(m->swf)
		{
			GetGame()->Log("Movie ptr of '", m->name, "' is ", m->swf->get_root_movie());
			static auto res = game->renderer->GetResolution();
		//	GetGame()->Log("Updating swf '", m->name, "', movie type: ", Type(m->swf->m_movie.get()));
			GetGame()->Log("Setting movie res '"+m->name+"'. ", lexical_cast<string>(res));
		//	m->swf->set_display_viewport(0, 0, res.x, res.y);
			GetGame()->Log("Setting back alpha '"+m->name+"'.");
			m->swf->set_background_alpha(0.1f);
			GetGame()->Log("Notifying mouse pos '"+m->name+"'.");
			res = game->input->GetMousePos();
			m->swf->notify_mouse_state(res.x,res.y,0);
			GetGame()->Log("Updating movie '"+m->name+"', at delta ", game->time->deltaTime, ".");
			m->swf->advance(game->time->deltaTime);
			GetGame()->Log("Rendering movie '"+m->name+"'.");
			m->swf->display();
		}
		else
		{
		//	GetGame()->Log("Setting root swf to player root movie.");
		//	m->swf = player->get_root();
		}
	}
}

void UserInterface::Deinit()
{
	gameswf::set_render_handler(nullptr);
}

void UserInterface::Save(Properties *prop) const
{

}

void UserInterface::Serialize(Properties *prop)
{

}

void UserInterface::OnKey(uint16 key, int mode)
{

	gameswf::key::code newKey = gameswf::key::INVALID;

	if(key == Key["Esc"]) { newKey = gameswf::key::ESCAPE; }
	else if(key == Key["f1"]) { newKey = gameswf::key::F1; }
	else if(key == Key["f2"]) { newKey = gameswf::key::F2; }
	else if(key == Key["f3"]) { newKey = gameswf::key::F3; }
	else if(key == Key["f4"]) { newKey = gameswf::key::F4; }
	else if(key == Key["f5"]) { newKey = gameswf::key::F5; }
	else if(key == Key["f6"]) { newKey = gameswf::key::F6; }
	else if(key == Key["f7"]) { newKey = gameswf::key::F7; }
	else if(key == Key["f8"]) { newKey = gameswf::key::F8; }
	else if(key == Key["f9"]) { newKey = gameswf::key::F9; }
	else if(key == Key["f10"]) { newKey = gameswf::key::F10; }
	else if(key == Key["f11"]) { newKey = gameswf::key::F11; }
	else if(key == Key["f12"]) { newKey = gameswf::key::F12; }
	else if(key == Key["Up"]) { newKey = gameswf::key::UP; }
	else if(key == Key["Down"]) { newKey = gameswf::key::DOWN; }
	else if(key == Key["Left"]) { newKey = gameswf::key::LEFT; }
	else if(key == Key["Right"]) { newKey = gameswf::key::RIGHT; }
	else if(key == Key["LShift"]) { newKey = gameswf::key::SHIFT; }
	else if(key == Key["RShift"]) { newKey = gameswf::key::SHIFT; }
	else if(key == Key["LCtrl"]) { newKey = gameswf::key::CONTROL; }
	else if(key == Key["RCtrl"]) { newKey = gameswf::key::CONTROL; }
	else if(key == Key["LAlt"]) { newKey = gameswf::key::ALT; }
	else if(key == Key["RAlt"]) { newKey = gameswf::key::ALT; }
	else if(key == Key["Tab"]) { newKey = gameswf::key::TAB; }
	else if(key == Key["Enter"]) { newKey = gameswf::key::ENTER; }
	else if(key == Key["Backspace"]) { newKey = gameswf::key::BACKSPACE; }
	else if(key == Key["Insert"]) { newKey = gameswf::key::INSERT; }
//	else if(key == Key["Delete"]) { newKey = gameswf::key::DEL; }
	else if(key == Key["PageUp"]) { newKey = gameswf::key::PGUP; }
	else if(key == Key["PageDown"]) { newKey = gameswf::key::PGDN; }
	else if(key == Key["Home"]) { newKey = gameswf::key::HOME; }
	else if(key == Key["End"]) { newKey = gameswf::key::END; }
	else if(key == Key["NumEnter"]) { newKey = gameswf::key::KP_ENTER; }
	else { newKey = (gameswf::key::code)key; }

	if(newKey != gameswf::key::INVALID)
	{
		player->notify_key_event(newKey, mode);
	}
}

void UserInterface::OnChar(char key)
{
}

void UserInterface::OnMouseButton(uint16 btn, int mode)
{
}

void UserInterface::OnMouseMove(int x, int y)
{
	/// @todo Only do for active movies? Decide how/when to set m.active
	for(auto& m : movies)
	{
		m->swf->notify_mouse_state(x, y, 0);
	}
}

void UserInterface::OnWindowResize(uint width, uint height)
{
	/// @todo Give movies a bool to do this or not. (based on linkage to screen?)
	for(auto& m : movies)
	{
		m->swf->set_display_viewport(0, 0, width, height);
	}
}

UserInterface::Movie *UserInterface::SerializeMovie(string name, string path)
{
	Movie* m = new Movie(name, player->load_file(path.c_str()));
	movies.push_back(m);
	player->set_root(m->swf);
	GetGame()->Log("Movie ptr of '", m->name, "' is ", m->swf->get_root_movie());
	return m;
}

UserInterface::Movie *UserInterface::GetMovie(string name)
{
	for(auto& m : movies)
	{
		if(m->name == name)
		{
			return m;
		}
	}
	return nullptr;
}

UserInterface::Movie::Movie(string name, gameswf::root *swf) :
	name(name), swf(swf)
{
}

UserInterface::Movie::~Movie()
{
}

void UserInterface::Movie::Play()
{
	swf->set_play_state(gameswf::character::PLAY);
}

void UserInterface::Movie::Pause()
{
	swf->set_play_state(gameswf::character::STOP);
}
*/

// CEGUI
/*
void UserInterface::CreateWindow(Properties* prop, CEGUI::Window* parent)
{
	CEGUI::Window* win = windowManager->createWindow(prop->Value("type"), prop->name);

	string type = type::Descope(type::Name(win));
	if(type == "PushButton")
	{
		win->subscribeEvent(CEGUI::PushButton::EventClicked, &UserInterface::OnClickButton, this);
	}
	else if(type == "FrameWindow")
	{
	//	win->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, &UserInterface::OnCloseWindow, this);
	}

	if(!parent)
	{
		rootWindow->addChild(win);
	}
	else
	{
		parent->addChild(win);
	}

	for(auto& val : prop->values)
	{
		if(val.first == "type")
		{
			continue;
		}
		GetGame()->Log("UserInterface::CreateWindow: Setting Property '"+val.first+"' to '"+val.second+"'.");
		win->setProperty(val.first, val.second);
	}

	for(auto& child : prop->children)
	{
		if(child->IsType("window")) CreateWindow(child, win);
	}

}

void UserInterface::Serialize(Properties *prop)
{
	if(!prop) return;
	Subsystem::Serialize(prop);
	GetGame()->Log("UserInterface::Serialize: Touch.");
	deque<string> schemes;
	string pfont = "";
	string pcursor = "";
	string file = "";
	prop->Value("schemes", schemes);
	prop->Value("font", pfont);
	prop->Value("cursor", pcursor);
	prop->Value("file", file);
	prop->Value("rootFolder", rootResFolder);

	GetGame()->Log("UserInterface, file to load: '"+file+"'.");
	if(file != "")
	{
		LoadFile(file);
	}

	for(auto& i : schemes)
	{
		GetGame()->Log("Attempting to load scheme '"+i+"'.");
		CEGUI::SchemeManager::getSingleton().createFromFile(i+".scheme", "schemes");
	}

	if(!pfont.empty())
	{
		CEGUI::Font& font = CEGUI::FontManager::getSingleton().createFromFile(pfont+".font");
		context->setDefaultFont(&font);
	}

	if(!pcursor.empty())
	{
		context->getPointerIndicator().setDefaultImage(pcursor);
	}

	context->setDefaultTooltipType(prop->Get<string>("tooltip"));

	for(auto& pw : prop->children)
	{
		if(pw->type == "window")
			CreateWindow(pw);
	}
}

void UserInterface::Save(Properties* prop) const
{
	Subsystem::Save(prop);
}

void UserInterface::Init()
{
	//	Subsystem::Init();

	if(!system)
	{
		renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
		windowManager = CEGUI::WindowManager::getSingletonPtr();
		system = CEGUI::System::getSingletonPtr();
		context = &system->getDefaultGUIContext();
		input = new CEGUI::InputAggregator(context);
	}

//	renderer->enableExtraStateSettings(true);

	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(system->getResourceProvider());
	rp->setResourceGroupDirectory("root", game->GetAssetPath(rootResFolder).string());
	rp->setDefaultResourceGroup("root");

	rp->setResourceGroupDirectory("imagesets", game->GetAssetPath(rootResFolder+"/imagesets").string());
	rp->setResourceGroupDirectory("schemes", game->GetAssetPath(rootResFolder+"/schemes").string());
	rp->setResourceGroupDirectory("xml_schemas", game->GetAssetPath(rootResFolder+"/xml_schemas").string());
	rp->setResourceGroupDirectory("styles", game->GetAssetPath(rootResFolder+"/styles").string());
	rp->setResourceGroupDirectory("layouts", game->GetAssetPath(rootResFolder+"/layouts").string());
	rp->setResourceGroupDirectory("fonts", game->GetAssetPath(rootResFolder+"/fonts").string());
	rp->setResourceGroupDirectory("looknfeel", game->GetAssetPath(rootResFolder+"/looks").string());

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::Font::setDefaultResourceGroup("fonts");
//	CEGUI::TinyXMLParser::setSchemaDefaultResourceGroup("xml_schemas");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");

	if(!rootWindow)
	{
		rootWindow = windowManager->createWindow("DefaultWindow", "Root");
	}
	context->setRootWindow(rootWindow);

	context->getPointerIndicator().setInitialPointerPosition(vec2_cast<CEGUI::Vector2f>((vec2)game->input->GetMousePos()));

	ConnectEvent(SENDER(game->input, KeyPress), RECEIVER(this, OnKey));
	ConnectEvent(SENDER(game->input, CharPress), RECEIVER(this, OnChar));
	ConnectEvent(SENDER(game->input, MouseButton), RECEIVER(this, OnMouseButton));
	ConnectEvent(SENDER(game->input, MouseMove), RECEIVER(this, OnMouseMove));
	ConnectEvent(SENDER(game->renderer, SetResolution), RECEIVER(this, OnWindowResize));

	GetGame()->Log("UserInterface::Init: Calling Subsystem::Init at end.");

	Subsystem::Init();
}

void UserInterface::Update()
{
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	system->injectTimePulse(game->time->deltaTime);
	context->injectTimePulse(game->time->deltaTime);

	if(resized)
	{
		system->notifyDisplaySizeChanged(CEGUI::Sizef(size.x, size.y));
		resized = false;
	}

	renderer->beginRendering();
	context->draw();
	renderer->endRendering();
	windowManager->cleanDeadPool();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UserInterface::Deinit()
{
	system->destroy();
	delete input;
}

void UserInterface::RemoveWindow(string fileName)
{
}

void UserInterface::OnKey(uint16 key, int mode)
{
	CEGUI::Key::Scan newKey = CEGUI::Key::Unknown;

	if(key == Key["Esc"]) { newKey = CEGUI::Key::Escape; }
	else if(key == Key["f1"]) { newKey = CEGUI::Key::F1; }
	else if(key == Key["f2"]) { newKey = CEGUI::Key::F2; }
	else if(key == Key["f3"]) { newKey = CEGUI::Key::F3; }
	else if(key == Key["f4"]) { newKey = CEGUI::Key::F4; }
	else if(key == Key["f5"]) { newKey = CEGUI::Key::F5; }
	else if(key == Key["f6"]) { newKey = CEGUI::Key::F6; }
	else if(key == Key["f7"]) { newKey = CEGUI::Key::F7; }
	else if(key == Key["f8"]) { newKey = CEGUI::Key::F8; }
	else if(key == Key["f9"]) { newKey = CEGUI::Key::F9; }
	else if(key == Key["f10"]) { newKey = CEGUI::Key::F10; }
	else if(key == Key["f11"]) { newKey = CEGUI::Key::F11; }
	else if(key == Key["f12"]) { newKey = CEGUI::Key::F12; }
	else if(key == Key["Up"]) { newKey = CEGUI::Key::ArrowUp; }
	else if(key == Key["Down"]) { newKey = CEGUI::Key::ArrowDown; }
	else if(key == Key["Left"]) { newKey = CEGUI::Key::ArrowLeft; }
	else if(key == Key["Right"]) { newKey = CEGUI::Key::ArrowRight; }
	else if(key == Key["LShift"]) { newKey = CEGUI::Key::LeftShift; }
	else if(key == Key["RShift"]) { newKey = CEGUI::Key::RightShift; }
	else if(key == Key["LCtrl"]) { newKey = CEGUI::Key::LeftControl; }
	else if(key == Key["RCtrl"]) { newKey = CEGUI::Key::RightControl; }
	else if(key == Key["LAlt"]) { newKey = CEGUI::Key::LeftAlt; }
	else if(key == Key["RAlt"]) { newKey = CEGUI::Key::RightAlt; }
	else if(key == Key["Tab"]) { newKey = CEGUI::Key::Tab; }
	else if(key == Key["Enter"]) { newKey = CEGUI::Key::Return; }
	else if(key == Key["Backspace"]) { newKey = CEGUI::Key::Backspace; }
	else if(key == Key["Insert"]) { newKey = CEGUI::Key::Insert; }
	else if(key == Key["Delete"]) { newKey = CEGUI::Key::Delete; }
	else if(key == Key["PageUp"]) { newKey = CEGUI::Key::PageUp; }
	else if(key == Key["PageDown"]) { newKey = CEGUI::Key::PageDown; }
	else if(key == Key["Home"]) { newKey = CEGUI::Key::Home; }
	else if(key == Key["End"]) { newKey = CEGUI::Key::End; }
	else if(key == Key["NumEnter"]) { newKey = CEGUI::Key::NumpadEnter; }
	else { newKey = (CEGUI::Key::Scan)key; }

	if(mode == GLFW_PRESS)
	{
		if(newKey != CEGUI::Key::Unknown)
		{
			input->injectKeyDown(newKey);
		}
	}
	else
	{
		if(newKey != CEGUI::Key::Unknown)
		{
			input->injectKeyUp(newKey);
		}
	}
}

void UserInterface::OnChar(char key)
{
	input->injectChar(key);
}

void UserInterface::OnMouseButton(uint16 btn, int mode)
{
	CEGUI::MouseButton newBtn = CEGUI::LeftButton;
	if(btn == Key["MouseLeft"])
	{
		newBtn = CEGUI::LeftButton;
	}
	else if(btn == Key["MouseRight"])
	{
		newBtn = CEGUI::RightButton;
	}
	else if(btn == Key["MouseMiddle"])
	{
		newBtn = CEGUI::MiddleButton;
	}

	if(mode == GLFW_PRESS)
	{
		if(newBtn != CEGUI::NoButton)
		{
			input->injectMouseButtonDown(newBtn);
		}
	}
	else
	{
		if(newBtn != CEGUI::NoButton)
		{
			input->injectMouseButtonUp(newBtn);

		}
	}
}

void UserInterface::OnMouseMove(vec2 pos)
{
	input->injectMousePosition(pos.x, pos.y);
//	context->getMouseCursor().setPosition(vec2_cast<CEGUI::Vector2f>(vec2(x,y)));
//	context->getPointerIndicator().setPosition(vec2_cast<CEGUI::Vector2f>(pos));
}

void UserInterface::OnWindowResize(int width, int height)
{
	GetGame()->Log("UserInterface::OnWindowResize(): Touch.");
	if(system)
	{
		GetGame()->Log("UserInterface::OnWindowResize(): There is in fact a system");
		resized = true;
		size = vec2(width, height);
		GetGame()->Log("UserInterface::OnWindowResize(): Resizing to ", size);
	//	system->notifyDisplaySizeChanged(CEGUI::Sizef((float)size.x, (float)size.y));
	}
}

bool UserInterface::OnClickButton(const CEGUI::EventArgs &evt)
{
	GetGame()->Log("BUTTON CLICKED!");
	const CEGUI::WindowEventArgs* args = dynamic_cast<const CEGUI::WindowEventArgs*>(&evt);
	if(!args) return false;

	TriggerEvent("ClickButton", (CEGUI::Window*)args->window);
	GetGame()->Log("Triggad");
	return true;
}

bool UserInterface::OnCloseWindow(const CEGUI::EventArgs &evt)
{
	const CEGUI::WindowEventArgs* args = dynamic_cast<const CEGUI::WindowEventArgs*>(&evt);
	if(!args) return false;

	TriggerEvent("CloseWindow", args->window);

	args->window->destroy();
}
*/

// generic
void UserInterface::Init()
{
	LoadFile("settings/userInterface.prop");
}

void UserInterface::Update()
{
}

void UserInterface::Deinit()
{
}




#else // if we are mobile...

void UserInterface::CreateWindow(Properties* prop, CEGUI::Window* parent)
{
}

void UserInterface::Serialize(Properties *prop)
{
}

void UserInterface::Save(Properties* prop) const
{
	Subsystem::Save(prop);
}

void UserInterface::Init()
{
}

void UserInterface::Update()
{
}

void UserInterface::Deinit()
{
}

void UserInterface::RemoveWindow(string fileName)
{
}

void UserInterface::OnKey(uint16 key, int mode)
{
}

void UserInterface::OnChar(char key)
{
}

void UserInterface::OnMouseButton(uint16 btn, int mode)
{
}

void UserInterface::OnMouseMove(int x, int y)
{
}

void UserInterface::OnWindowResize(uint width, uint height)
{
}

#endif


}
