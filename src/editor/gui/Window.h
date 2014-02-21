
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Rect.h>
#include <lfant/ptr.h>
#include <lfant/util/qumap.h>
#include <lfant/Entity.h>
#include <editor/gui/GLCanvas.h>

// External
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <map>

class wxAuiManager;
class wxAuiNotebook;
class wxButton;
class wxWindow;
class wxPanel;
class wxBoxSizer;
class wxGLCanvas;
class wxGLContext;
class wxGrid;

namespace lfant {

class Game;

namespace editor {
namespace gui {

class Inspector;
class EntityTree;

class MenuIds
{
public:
	enum File
	{
		Quit = 1,
		About
	};
};

class Orient
{
public:
	enum Orientation
	{
		Vertical = wxVERTICAL,
		Horizontal = wxHORIZONTAL
	};
};

class Window : public wxFrame, public lfant::Object
{
	friend class Inspector;
public:

	Window(string title, Rect rect);
	virtual ~Window();

	virtual void Init();
	virtual void Deinit();
	void Render();

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnLoadProject(wxCommandEvent& event);

	void SaveScene(wxCommandEvent& event);

	static int GetId(string name = "");
	wxWindow* GetWidget(string name);
	int AddId(string name);

	wxAuiNotebook* AddPane(string name, string label);
	wxAuiNotebook* GetPane(string name);

	wxAuiNotebook* StartAuiNotebook(string name, string label = "", int pos = -1);
	wxPanel* StartPanel(string name, string label = "");
	wxBoxSizer* StartBoxSizer(int orient);

	wxMenu* StartMenu(string label);
	void EndMenu();
	void AddMenuItem(string name, string label, wxObjectEventFunction evt = nullptr);
	void AddMenuSeparator();

	void StartWidget(wxWindow* widget, int flags = -1);
	void AddWidget(wxWindow* parent, wxWindow* widget, int flags = -1, int proportion = 0);
	void AddWidget(wxWindow* widget, int flags = -1, int proportion = 0);
	void EndWidget();
	wxWindow* GetLastWidget(int back = 1);
	wxObject* GetLastObject(int back = 1);

protected:

	void FindAddEntityNode(Entity* entity);
	void AddEntityNode(wxTreeItemId& parent, Entity* entity);

	wxTreeItemId FindEntityInTree(Entity* entity);
	wxTreeItemId FindEntityInTree(wxTreeItemId parent, Entity* entity, void* vali);
	void ClearTree(wxTreeItemId id = 0);

	void AddEntityComponent(wxCommandEvent& evt);
	void RefreshEntity(wxCommandEvent& evt);

	void AddEntity(wxCommandEvent& evt);

	static string ConvertString(const wxString& str);

	void AddPropToGrid(wxWindow* parent, Properties* prop);

	static qumap<string, int> ids;
	static int currId;
	wxAuiManager* manager = nullptr;
	std::deque<wxAuiNotebook*> notebooks;

	std::deque<wxObject*> widgetStack;
	std::deque<wxMenu*> menuStack;

	wxMenuBar* menuBar = nullptr;
	GLCanvas* canvas = nullptr;

	EntityTree* tree = nullptr;
	Inspector* inspector = nullptr;


private:
//	DECLARE_EVENT_TABLE()
};

}
}
}
