
#include <editor/gui/App.h>

// Internal
#include <editor/gui/Window.h>
#include <lfant/Rect.h>

// External

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
	window = new Window("lfant", Rect(100, 50, 450, 340));
	window->Show(true);
	window->Init();
	SetTopWindow(window);
	return true;
}

int App::MainLoop()
{
	
}

}
}
}