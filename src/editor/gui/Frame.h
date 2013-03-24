
#pragma once
#include <lfant/stdafx.h>

// External
#include <deque>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/event.h>

// Internal
#include <editor/gui/Types.h>
#include <lfant/ptr.h>

class wxCommandEvent;

namespace lfant
{
namespace editor
{
namespace gui
{

/**
 *
 */
class Frame : public FrameBase
{
	friend class App;
public:
	enum
	{
		ID_Quit = 1,
		ID_About,
	};

	Frame(const string& title, const ivec2& pos, const ivec2& size);
	~Frame();

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	MenuBar* menuBar;
	deque< ptr<Menu> > menus;

};

}
}
}
