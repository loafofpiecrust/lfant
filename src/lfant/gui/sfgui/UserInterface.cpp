
#include "lfant/gui/sfgui/UserInterface.h"
#include <lfant/Game.h>
#include <lfant/Window.h>
#include <lfant/Time.h>

#include <SFGUI/Widget.hpp>
#include <SFGUI/Button.hpp>

namespace lfant {
namespace gui {
namespace sfgui {

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::Init()
{
	lfant::UserInterface::Init();
	
	ConnectEvent(game->window.get(), "OnEvent", RECEIVER(this, OnEvent));
}

void UserInterface::OnEvent(sf::Event evt)
{
	desktop.HandleEvent(evt);
}

void UserInterface::Serialize(Properties* prop)
{
	lfant::UserInterface::Serialize(prop);
	
	string f = "";
	prop->Value("file", f);
	if(!f.empty())
	{
		LoadFile(f);
	}
	
	for(auto& c : prop->children)
	{
		if(c->IsType("Button"))
		{
			auto win = sfg::Window::Create();
			auto btn = sfg::Button::Create(c->name);
			win->Add(btn);
			desktop.Add(win);
		}
	}
}

void UserInterface::Update()
{
	desktop.Update(game->time->deltaTime);
	Render();
}

void UserInterface::Render()
{
	gui.Display(*game->window->GetHandle());
}

}
}
}