
#include <editor/gui/DirTree.h>

// internal

// external
#include <vector>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

namespace lfant {
namespace editor {
namespace gui {

DirTree::DirTree(wxWindow* parent, const wxWindowID id, string rootDir, const wxString& name) :
	wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE, wxDefaultValidator, name)
{
	if(rootDir != "/") SetRoot(rootDir);
}

void DirTree::SetRoot(string dir)
{
	path rootPath(dir);
	wxTreeItemId root = AddRoot(rootPath.filename().string());
	ExpandDir(root, dir);
}

void DirTree::ExpandDir(wxTreeItemId parent, string dir)
{
	path root(dir);
	if(!is_directory(root)) return;

	vector<path> children;
	copy(directory_iterator(root), directory_iterator(), back_inserter(children));
	sort(children.begin(), children.end());

	for(vector<path>::const_iterator it(children.begin()); it != children.end(); ++it)
	{
		wxTreeItemId folder = AppendItem(parent, (*it).filename().string());
		ExpandDir(folder, (*it).string());
	}

}

}
}
}