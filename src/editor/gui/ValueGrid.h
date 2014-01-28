
#pragma once
#include <lfant/stdafx.h>

// internal

// external
#include <wx/grid.h>

namespace lfant {
namespace editor {
namespace gui {

class ValueGrid : public wxGrid
{
public:
	ValueGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, 
				const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, 
				const wxString& name = wxPanelNameStr);

	void OnSize(wxSizeEvent &evt);

	void SizeColumns();

protected:

private:

	DECLARE_EVENT_TABLE();
};

}
}
}