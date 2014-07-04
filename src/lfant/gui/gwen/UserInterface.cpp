

#include "lfant/gui/gwen/OpenGL.h"


#include <Gwen/Gwen.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Controls.h>
#include <Gwen/Controls/Canvas.h>
#include <Gwen/Controls/DockedTabControl.h>
#include <Gwen/Controls/WindowControl.h>
//#include <Gwen/Input/GLFW.h>
//#include <SFML/Graphics/RenderWindow.hpp>
//#include <SFML/Graphics/Text.hpp>
//#include <SFML/Graphics/CircleShape.hpp>

//#include <ft2build.h>
//#include <freetype/freetype.h>

#include <GL/glew.h>

#include "lfant/gui/gwen/UserInterface.h"
//#include "SFML2.h"

#include "lfant/Game.h"
#include "lfant/Window.h"
#include "lfant/FileSystem.h"
#include <lfant/Console.h>
#include "lfant/Input.h"
#include "lfant/Renderer.h"

namespace lfant {
namespace gui {
namespace gwen {

UserInterface::UserInterface(Game* game) :
	lfant::UserInterface(game)
{
}

UserInterface::~UserInterface()
{

}
Gwen::Controls::Label* fpsCounter;
void UserInterface::Init()
{
//	auto err = FT_Init_FreeType(&(FT_Library)freetype);
//	if(err)
	{
	//	GetGame()->Log("Freetype error. o noes.");
	}
	renderer = new Gwen::Renderer::OpenGL(game->window.get());
//	renderer = new Gwen::Renderer::SFML2(*(GetGame()->window->GetHandle()));
	renderer->Init();

	skin = new Gwen::Skin::TexturedBase(renderer);
	skin->Init("gui/images/gmod_skin.png"); /// @todo Loading of skin path from config

	skin->SetDefaultFont(Gwen::Utility::StringToUnicode(game->GetAssetPath("gui/fonts/Ubuntu-R.ttf").string()), 11);

	auto res = game->window->GetSize();
	canvas = new Gwen::Controls::Canvas(skin);
	canvas->SetSize(res.x, res.y);
	canvas->SetDrawBackground(false);
	canvas->SetBackgroundColor(Gwen::Color(150, 170, 170, 255));

	canvas->SetHidden(false);

	auto widget = new Gwen::Controls::Button(canvas, "hihi");
	widget->SetPos(10, 10);
	widget->SetText("yosup");

	auto box = new Gwen::Controls::WindowControl(canvas, "chat");
//	auto box = new Gwen::Controls::GroupBox(canvas, "chat");
	box->SetPos({30,30});
	box->SetClosable(false);
	box->SetClampMovement(false);
	box->SetSize({100,120});
	{
		auto history = new Gwen::Controls::TextBoxMultiline(box, "chat.history");
		history->SetPos({10,10});
		history->SetSize({60,45});
		history->SetEditable(false);
		history->SetKeyboardInputEnabled(false);

		auto entry = new Gwen::Controls::TextBox(box, "chat.entry");
		entry->SetPos({10,55});
		entry->SetSize({60,20});
		history->SetEditable(true);

		auto submit = new Gwen::Controls::Button(box, "chat.submit");
		submit->SetPos({10,85});
		submit->SetSize({40,20});
	}

//	input = new Gwen::Input::SFML;
//	input = new Gwen::Input::GLFW;
//	input->Initialize(canvas);

//	ConnectEvent(SENDER(game->input, KeyPress), RECEIVER(this, OnKey));
//	ConnectEvent(SENDER(game->input, CharPress), RECEIVER(this, OnChar));
//	ConnectEvent(SENDER(game->input, MouseButton), RECEIVER(this, OnMouseButton));
//	ConnectEvent(SENDER(game->input, MouseMove), RECEIVER(this, OnMouseMove));
	ConnectEvent(SENDER(game->renderer, SetResolution), RECEIVER(this, OnWindowResize));
}

void UserInterface::Update()
{
	lfant::UserInterface::Update();
	Render();
}

void UserInterface::Render()
{
//	glDisable(GL_DEPTH_TEST);
//	GetGame()->window->GetHandle()->pushGLStates();
//	glDisable(GL_DEPTH_TEST);
	canvas->RenderCanvas();

//	GetGame()->window->GetHandle()->popGLStates();
//	glEnable(GL_DEPTH_TEST);
}

void UserInterface::Deinit()
{
	lfant::UserInterface::Deinit();
}

void* UserInterface::GetRoot()
{
	return (void*)canvas;
}

void UserInterface::OnKey(uint16 key, int mode, int mods)
{
	canvas->InputKey(key, mode);
}

void UserInterface::OnChar(char key)
{
	canvas->InputCharacter(key);
}

void UserInterface::OnMouseButton(uint16 btn, int mode, int mods)
{
	canvas->InputMouseButton(btn, mode);
}

void UserInterface::OnMouseMove(ivec2 pos)
{
	ivec2 delta = pos - prevMousePos;
//	ivec2 res = game->window->GetSize();
	canvas->InputMouseMoved(pos.x, pos.y, delta.x, delta.y);
	prevMousePos = pos;

	/// @todo Implement cursor here
}

void UserInterface::OnWindowResize(int width, int height)
{
	canvas->SetSize(width, height);
}


}
}
}
