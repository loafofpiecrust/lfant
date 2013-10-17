
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Rect.h>
#include <lfant/ptr.h>
#include <lfant/util/qumap.h>
#include <lfant/Entity.h>

// External
#include <wx/frame.h>
#include <map>

class wxAuiManager;
class wxAuiNotebook;
class wxButton;
class wxWindow;
class wxPanel;
class wxBoxSizer;
class wxGLCanvas;
class wxGLContext;

namespace lfant {
namespace editor {
namespace gui {

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

class Window : public wxFrame
{
public:
	Window(string title, Rect rect);
	virtual ~Window();

	virtual void Init();

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	int GetId(string name = "");
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
	void AddWidget(wxWindow* widget, int flags = -1);
	void EndWidget();
	wxWindow* GetLastWidget(int back = 1);
	wxObject* GetLastObject(int back = 1);

protected:
	qumap<string, int> ids;
	int currId = 1;
	ptr<wxAuiManager> manager;
	deque<ptr<wxAuiNotebook>> notebooks;

	deque<wxObject*> widgetStack;
	deque<wxMenu*> menuStack;

	ptr<wxMenuBar> menuBar;
	ptr<wxGLCanvas> canvas;
	ptr<wxGLContext> context;

	Entity* currentEnt = nullptr;

private:
};

}
}
}