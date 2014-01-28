
#include <editor/gui/App.h>

// Internal
#include <editor/gui/Window.h>
#include <lfant/Rect.h>
#include <lfant/Console.h>
#include <editor/Editor.h>

// External
#include <wx/wx.h>

IMPLEMENT_APP(lfant::editor::gui::App)

namespace lfant {
namespace editor {
namespace gui {

App::App()
{
}

App::~App()
{
}

bool App::OnInit()
{
//	Log("Called OnInit, ", this);
	game = new Editor;
	game->standAlone = false;
	window = new Window("lfant", Rect(100, 50, 1280, 720));
	window->Show(true);
	window->Init();
	SetTopWindow(window);

//	game->Init();
	
	ActivateRenderLoop(true);

//	Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(App::OnIdle) );
	return true;
}

void App::ActivateRenderLoop(bool on)
{
//	if(!this) Log("Um this gone? ", this); return;
	if(on && !render_loop_on)
	{
		Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(App::OnIdle) );
		render_loop_on = true;
	}
	else if(!on && render_loop_on)
	{
		Disconnect( wxEVT_IDLE, wxIdleEventHandler(App::OnIdle) );
		render_loop_on = false;
	}
}

void App::OnIdle(wxIdleEvent& evt)
{
//	if(render_loop_on)
	{
	//	std::cout << "App::OnIdle: Touch.\n";
	//	Log("App::OnIdle: Touch.");
	//  drawPane->paintNow();
		window->Render();
		game->Update();
		evt.RequestMore(); // render continuously, not only once on idle
	}
}

}
}
}