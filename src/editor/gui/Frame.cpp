
#include <editor/gui/Frame.h>

// External
#include <wx/msgdlg.h>

// Internal
#include <lfant/TypeInfo.h>

namespace lfant
{
namespace editor
{
namespace gui
{

Frame::Frame(const string &title, const ivec2 &pos, const ivec2 &size) :
	FrameBase(nullptr, -1, _(title.c_str()), vec2_cast<wxPoint>(pos), vec2_cast<wxSize>(size))
{
	menuBar = new MenuBar;

	Menu *menuFile = new Menu;
	menus.push_front(menuFile);

	menuFile->Append( ID_About, _("&About...") );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Quit, _("E&xit") );

	menuBar->Append(menuFile, _("&File") );

	SetMenuBar(menuBar);

	CreateStatusBar();

	SetStatusText( _("Welcome to wxWidgets!") );
}

Frame::~Frame()
{
}

void Frame::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox( "wxWidgets Hello World example.", "About Hello World", wxOK|wxICON_INFORMATION, this );
}

}
}
}
