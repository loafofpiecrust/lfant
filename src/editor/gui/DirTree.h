
#include <lfant/stdafx.h>

// internal

// external
#include <wx/treectrl.h>

namespace lfant {
namespace editor {
namespace gui {

class DirTree : public wxTreeCtrl
{
public:
	DirTree(wxWindow* parent, const wxWindowID id = wxID_ANY, string rootDir = "/", const wxString& name = wxTreeCtrlNameStr);

	void SetRoot(string dir);

protected:
	void ExpandDir(wxTreeItemId parent, string dir);

private:
};

}
}
}