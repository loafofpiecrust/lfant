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

// Internal
#include <lfant/UserInterface.h>
#include <lfant/Thread.h>

#include <lfant/Game.h>
#include <lfant/FileSystem.h>
#include <lfant/Console.h>
#include <lfant/Input.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Settings.h>
#include <lfant/Time.h>
#include <lfant/Thread.h>
#include <lfant/Renderer.h>

// External
//#include <Rocket/Core.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL3/Renderer.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/DefaultResourceProvider.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI/XMLParserModules/TinyXML/XMLParser.h>

namespace lfant
{

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

// CEGUI

void UserInterface::CreateWindow(Properties* prop, CEGUI::Window* parent)
{
	CEGUI::Window* win = windowManager->createWindow(prop->Get<string>("type"), prop->id);

	string type = Split(prop->Get<string>("type"), "/")[1];
	if(type == "Button")
	{
		win->subscribeEvent(CEGUI::PushButton::EventClicked, &UserInterface::OnClickButton, this);
	}
	else if(type == "FrameWindow")
	win->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, &UserInterface::OnCloseWindow, this);

	if(!parent)
	{
		rootWindow->addChild(win);
	}
	else
	{
		parent->addChild(win);
	}

	for(auto& val : prop->GetValues())
	{
		if(val.first == "type")
		{
			continue;
		}
	//	Log("UserInterface::CreateWindow: Setting Property '"+val.first+"' to '"+val.second+"'.");
		win->setProperty(val.first, val.second);
	}

	for(auto& child : prop->GetChildren("window"))
	{
		CreateWindow(child, win);
	}

}

void UserInterface::Load(Properties *prop)
{
	deque<string> schemes;
	string pfont = "";
	string pcursor = "";
	prop->Get("schemes", schemes);
	prop->Get("font", pfont);
	prop->Get("cursor", pcursor);

	for(auto& i : schemes)
	{
		Log("Attempting to load scheme '"+i+"'.");
		CEGUI::SchemeManager::getSingleton().createFromFile(i+".scheme", "schemes");
	}

	if(pfont != "")
	{
		CEGUI::Font& font = CEGUI::FontManager::getSingleton().createFromFile(pfont+".font");
		context->setDefaultFont(&font);
	}

	if(pcursor != "")
	{
		context->getMouseCursor().setDefaultImage(pcursor);
	}

	context->setDefaultTooltipType(prop->Get<string>("tooltip"));

	deque<Properties*> pwins = prop->GetChildren("window");
	for(Properties* pw : pwins)
	{
		CreateWindow(pw);
	}
}

void UserInterface::Init()
{
	//	Subsystem::Init();

	renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
	windowManager = CEGUI::WindowManager::getSingletonPtr();
	system = CEGUI::System::getSingletonPtr();
	context = &system->getDefaultGUIContext();

	renderer->enableExtraStateSettings(true);

	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(system->getResourceProvider());
	rp->setResourceGroupDirectory("assets", game->fileSystem->GetGamePath("").string());
	rp->setDefaultResourceGroup("assets");

	rp->setResourceGroupDirectory("imagesets", game->fileSystem->GetGamePath("gui/imagesets").string());
	rp->setResourceGroupDirectory("schemes", game->fileSystem->GetGamePath("gui/schemes").string());
	rp->setResourceGroupDirectory("xml_schemas", game->fileSystem->GetGamePath("gui/xml_schemas").string());
	rp->setResourceGroupDirectory("styles", game->fileSystem->GetGamePath("gui/styles").string());
	rp->setResourceGroupDirectory("layouts", game->fileSystem->GetGamePath("gui/layouts").string());
	rp->setResourceGroupDirectory("fonts", game->fileSystem->GetGamePath("gui/fonts").string());
	rp->setResourceGroupDirectory("looknfeel", game->fileSystem->GetGamePath("gui/looks").string());

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::Font::setDefaultResourceGroup("fonts");
//	CEGUI::TinyXMLParser::setSchemaDefaultResourceGroup("xml_schemas");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");

	rootWindow = windowManager->createWindow("DefaultWindow", "Root");
	context->setRootWindow(rootWindow);

	context->getMouseCursor().setInitialMousePosition(vec2_cast<CEGUI::Vector2f>(game->input->GetMousePos()));

	ConnectEvent(SENDER(game->input, KeyPress), RECEIVER(this, OnKey));
	ConnectEvent(SENDER(game->input, CharPress), RECEIVER(this, OnChar));
	ConnectEvent(SENDER(game->input, MouseButton), RECEIVER(this, OnMouseButton));
	ConnectEvent(SENDER(game->input, MouseMove), RECEIVER(this, OnMouseMove));
	ConnectEvent(SENDER(game->renderer, SetResolution), RECEIVER(this, OnWindowResize));

	Log("UserInterface::Init: Calling Subsystem::Init at end.");
	Subsystem::Init();
}

void UserInterface::Update()
{
	system->injectTimePulse(game->time->deltaTime);
	context->injectTimePulse(game->time->deltaTime);

	if(resized)
	{
		system->notifyDisplaySizeChanged(CEGUI::Sizef((float)size.x, (float)size.y));
		resized = false;
	}

	renderer->beginRendering();
	context->draw();
	renderer->endRendering();
	windowManager->cleanDeadPool();
}

void UserInterface::OnDestroy()
{
	system->destroy();
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

	if(mode == GLFW_PRESS)
	{
		if(newKey != CEGUI::Key::Unknown)
		{
			context->injectKeyDown(newKey);
		}
	}
	else
	{
		if(newKey != CEGUI::Key::Unknown)
		{
			context->injectKeyUp(newKey);
		}
	}
}

void UserInterface::OnChar(char key)
{
	context->injectChar(key);
}

void UserInterface::OnMouseButton(uint16 btn, int mode)
{
	CEGUI::MouseButton newBtn = CEGUI::NoButton;
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
			context->injectMouseButtonDown(newBtn);
		}
	}
	else
	{
		if(newBtn != CEGUI::NoButton)
		{
			context->injectMouseButtonUp(newBtn);
		}
	}
}

void UserInterface::OnMouseMove(int x, int y)
{
	context->injectMousePosition(x, y);
	//context->getMouseCursor().setPosition(vec2_cast<CEGUI::Vector2f>(vec2(x,y)));
}

void UserInterface::OnWindowResize(uint width, uint height)
{
	if(system)
	{
		resized = true;
		size = uvec2(width, height);
		//	rootWindow->setMaxSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	}
}

bool UserInterface::OnClickButton(const CEGUI::EventArgs &evt)
{
	const CEGUI::WindowEventArgs* args = dynamic_cast<const CEGUI::WindowEventArgs*>(&evt);
	if(!args) return false;

	TriggerEvent("ClickButton", args->window);
}

bool UserInterface::OnCloseWindow(const CEGUI::EventArgs &evt)
{
	const CEGUI::WindowEventArgs* args = dynamic_cast<const CEGUI::WindowEventArgs*>(&evt);
	if(!args) return false;

	TriggerEvent("CloseWindow", args->window);

	args->window->destroy();
}

}
