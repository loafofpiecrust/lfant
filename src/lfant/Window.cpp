
#include "lfant/Window.h"

#include "lfant/Game.h"
#include "lfant/Input.h"
#include "lfant/Renderer.h"
#include "lfant/Console.h"
#include "UserInterface.h"
#include "gui/gwen/UserInterface.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
//#include <Gwen/Input/SFML.h>
//#include <../src/SFML/Graphics/GLCheck.hpp>

namespace lfant {

Window::Window(Game* game) :
	Subsystem(game)
//	,window{new sf::RenderWindow}
{
//	window = {new sf::Window};
	glewExperimental = true;
}

Window::~Window()
{

}

GLFWwindow* Window::GetHandle()
{
	return handle;
}

void Window::Save(Properties* prop) const
{
	Subsystem::Save(prop);
}

void Window::Load(Properties* prop)
{
	Subsystem::Load(prop);

	prop->Get("fsaa", fsaa);
	prop->Get("version", version);

	ivec2 res {800,600};
	prop->Get("resolution", res);

	if(!IsOpen())
	{
		Open(res);
	}

	string title = "";
	prop->Get("title", title);
	SetTitle(title);

}

void Window::Init()
{
	if(!glfwInit())
	{
		// glfw failed to init
		exit(1);
		return;
	}

	if(!IsOpen())
	{
		Open({889,500});
	}

}


void Window::Update()
{
	lfant::Subsystem::Update();

	glfwPollEvents();
	if(glfwWindowShouldClose(handle))
	{
		// close..
	//	Close();
		game->Exit();
		// more?
	}

/*	static sf::Event evt;
	while(window->pollEvent(evt))
	{
		switch(evt.type)
		{
			case sf::Event::MouseMoved:
			{
				cursorPos = {evt.mouseMove.x, evt.mouseMove.y};
				GetGame()->input->OnMouseMove((vec2)cursorPos);
				break;
			}
			case sf::Event::KeyPressed:
			{
				GetGame()->input->OnKeyPress(evt.key.code, 1, 0);
				break;
			}
			case sf::Event::KeyReleased:
			{
				GetGame()->input->OnKeyPress(evt.key.code, 0, 0);
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				GetGame()->input->OnMouseButton(evt.mouseButton.button, 1, 0);
			}
			case sf::Event::MouseButtonReleased:
			{
				GetGame()->input->OnMouseButton(evt.mouseButton.button, 0, 0);
			}
			case sf::Event::Resized:
			{
				GetGame()->renderer->Resize({(int)evt.size.width, (int)evt.size.height});
				break;
			}
			case sf::Event::Closed:
			{
				Close();
				GetGame()->Exit();
				break;
			}
			default:
			{
				break;
			}
		}
		((gui::gwen::UserInterface*)GetGame()->userInterface.get())->input->ProcessMessage(evt);
	}*/
}

void Window::Render()
{
	glfwSwapBuffers(handle);
//	GetGame()->Log(glGetError());
//	window->display();
//	window->clear();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Deinit()
{
	lfant::Subsystem::Deinit();
	Close();
	glfwTerminate();
//	handle.reset();
//	GetGame()->Log("GL resources: ", sf::GlResource::getCount());
}

bool Window::IsOpen()
{
	return handle != nullptr;
}

void Window::Open(ivec2 res)
{
	if(!GetGame()->standAlone) return;

	GetGame()->Log("opening window");

	resolution = res;

	glfwWindowHint(GLFW_RESIZABLE, resizable);
//	glfwWindowHint(GLFW_FULLSCREEN, fullscreen);
	glfwWindowHint(GLFW_SAMPLES, fsaa);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	GetGame()->Log(glGetError());

	handle = glfwCreateWindow(res.x, res.y, "", nullptr, nullptr);

	if(!handle)
	{
		GetGame()->Log("Window failed to open, terminating");
		// ...
		glfwTerminate(); // ?
		return;
	}
	GetGame()->Log(glGetError());

	glfwSetWindowUserPointer(handle, this);
	SetCurrent();
	GetGame()->Log(glGetError());
//	SetVsync(1);

	if(needsInit)
	{
		glewExperimental = true;
		GetGame()->Log(glewGetErrorString(glewInit()));
		needsInit = false;
	}

	GetGame()->Log("Window opened, gl version ", glfwGetWindowAttrib(handle, GLFW_CONTEXT_VERSION_MAJOR), ".", glfwGetWindowAttrib(handle, GLFW_CONTEXT_VERSION_MINOR));

	glfwSetWindowSizeCallback(handle, &OnWindowResize);
	glfwSetWindowPosCallback(handle, &OnWindowMove);
	glfwSetCursorPosCallback(handle, &OnCursorMove);
	glfwSetCursorEnterCallback(handle, &OnCursorEnter);
	glfwSetScrollCallback(handle, &OnScroll);
	glfwSetKeyCallback(handle, &OnKey);
	glfwSetCharCallback(handle, &OnChar);
	glfwSetMouseButtonCallback(handle, &OnMouseButton);
	glfwSetFramebufferSizeCallback(handle, &OnFrameResize);

/*	uint32 style = sf::Style::Titlebar|sf::Style::Close;
	if(fullscreen)
	{
		style = sf::Style::Fullscreen;
	}
	sf::ContextSettings settings(32,8,fsaa,version.major,version.minor);
	window->create(sf::VideoMode(res.x, res.y), title, style, settings);

	if(!window->isOpen())
	{
		GetGame()->Log("Window failed to open");
		return;
	}

	window->setKeyRepeatEnabled(false);
	window->setActive(true);
	window->setFramerateLimit(100);

	if(needsInit)
	{
		GetGame()->Log(glewGetErrorString(glewInit()));
		needsInit = false;
	}*/
}

void Window::SetCurrent()
{
	if(!GetGame()->standAlone) return;
	glfwMakeContextCurrent(handle);
//	window->setActive(true);
}

void Window::Close()
{
	if(handle) glfwDestroyWindow(handle);
//	window->close();
}

bool Window::SetVsync(int enable)
{
	if(!GetGame()->standAlone) return false;
	glfwSwapInterval(enable);
//	window->setVerticalSyncEnabled(enable);
	return true;
}

void Window::SetTitle(string title)
{
	if(!GetGame()->standAlone)
	{
		this->title = title;
		return;
	}

	glfwSetWindowTitle(handle, title.c_str());
//	window->setTitle(title);
}

ivec2 Window::GetSize() const
{
//	return resolution;

/*	auto size = window->getSize();
	return {(int)size.x, (int)size.y};*/

	ivec2 pos;
	glfwGetWindowSize(handle, &pos.x, &pos.y);
	return pos;
}

void Window::Resize(ivec2 res)
{
	if(!GetGame()->standAlone)
	{
		resolution = res;
		return;
	}

	glfwSetWindowSize(handle, res.x, res.y);
//	window->setSize({(uint)res.x, (uint)res.y});
}

ivec2 Window::GetPosition() const
{
//	if(!GetGame()->standAlone) return position;

/*	auto pos = window->getPosition();
	return {pos.x, pos.y};*/

	ivec2 pos;
	glfwGetWindowPos(handle, &pos.x, &pos.y);
	return pos;
}

void Window::SetPosition(ivec2 pos)
{
	if(!GetGame()->standAlone)
	{
		position = pos;
		return;
	}

	glfwSetWindowPos(handle, pos.x, pos.y);
//	window->setPosition({pos.x, pos.y});
}

void Window::HideCursor(bool hide)
{

}

ivec2 Window::GetCursorPos() const
{
	if(!GetGame()->standAlone)
	{
		return cursorPos;
	}

/*	auto pos = sf::Mouse::getPosition();
	return {pos.x, pos.y};*/

	dvec2 pos;
	glfwGetCursorPos(handle, &pos.x, &pos.y);
	return vec2_cast<ivec2>(pos);
}

void Window::SetCursorPos(ivec2 pos)
{
	if(!GetGame()->standAlone)
	{
		cursorPos = pos;
		return;
	}

	glfwSetCursorPos(handle, (double)pos.x, (double)pos.y);
//	sf::Mouse::setPosition({pos.x, pos.y});
}

/*
 * 	Callbacks
 */

void Window::OnFrameResize(GLFWwindow* window, int width, int height)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->GetGame()->renderer->Resize({width, height});
}

void Window::OnWindowResize(GLFWwindow* window, int width, int height)
{
//	game->renderer->Resize(width, height);
}

void Window::OnWindowMove(GLFWwindow* window, int x, int y)
{
}

void Window::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->GetGame()->input->OnMouseButton(button, action, mods);
}

void Window::OnCursorMove(GLFWwindow* window, double x, double y)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->GetGame()->input->OnMouseMove(vec2(x, y));
}

void Window::OnCursorEnter(GLFWwindow* window, int entered)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->GetGame()->input->OnCursorEnter(entered);
}

void Window::OnScroll(GLFWwindow* window, double xoff, double yoff)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->GetGame()->input->OnScroll(vec2(xoff, yoff));
}

void Window::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->GetGame()->input->OnKeyPress(key, action, mods);
}

void Window::OnChar(GLFWwindow* window, uint letter)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->GetGame()->input->OnCharPress(letter);
}

}
