
#include <editor/gui/App.h>

// External
#include <wx/wx.h>

// Internal
#include <editor/gui/Frame.h>

namespace lfant
{
namespace editor
{
namespace gui
{

IMPLEMENT_APP(App)

App::App()
{
}

App::~App()
{
}

bool App::OnInit()
{
	frame = new Frame( "Hello World", ivec2(50, 50), ivec2(450, 350));

	frame->Connect( Frame::ID_Quit, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &Frame::OnQuit );
	frame->Connect( Frame::ID_About, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction) &Frame::OnAbout );

	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

}
}
}
