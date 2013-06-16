
#include <editor/gui/Window.h>

// Internal
#include <editor/Editor.h>
#include <lfant/Console.h>
#include <lfant/Renderer.h>

// External
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/glcanvas.h>

namespace lfant {
namespace editor {
namespace gui {

Window::Window(string title, Rect rect) :
	wxFrame(0, -1, wxString(title), wxPoint(rect.x, rect.y), wxSize(rect.width, rect.height))
{
}

Window::~Window()
{
}

void Window::Init()
{
	Connect(GetId("File.Quit"), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&Window::OnQuit);
	Connect(GetId("File.About"), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&Window::OnAbout);

	// Basic Setup
	menuBar = new wxMenuBar;
	/*
	wxMenu* fileMenu = new wxMenu;

	fileMenu->Append(GetId("file.newProject"), "New Project");
	fileMenu->Append(GetId("file.loadProject"), "Load Project...");
	fileMenu->Append(GetId("file.saveProject"), "Save Project");
	fileMenu->AppendSeparator();
	fileMenu->Append(GetId("file.newScene"), "&New Scene");
	fileMenu->Append(GetId("file.loadScene"), "Load Scene...");
	fileMenu->Append(GetId("file.saveScene"), "&Save Scene");
	fileMenu->AppendSeparator();
	fileMenu->Append(GetId("file.about"), "&About...");
	fileMenu->Append(GetId("file.quit"), "E&xit");
	*/

	StartMenu("&File");
	{
		AddMenuItem("File.NewProject", "New Project");
		AddMenuItem("File.LoadProject", "Load Project...");
		AddMenuItem("File.SaveProject", "Save Project");

		AddMenuSeparator();

		AddMenuItem("File.NewProject", "&New Scene");
		AddMenuItem("File.LoadProject", "Load Scene...");
		AddMenuItem("File.SaveProject", "&Save Scene");

		AddMenuSeparator();

		AddMenuItem("File.About", "About");
		AddMenuItem("File.Quit", "E&xit");
	}
	EndMenu();

	StartMenu("Edit");
	{
		AddMenuItem("Edit.Undo", "Undo");
		AddMenuItem("Edit.Redo", "Redo");

		AddMenuSeparator();

		AddMenuItem("Edit.Cut", "Cut");
		AddMenuItem("Edit.Copy", "&Copy");
		AddMenuItem("Edit.Paste", "Paste");
	}
	EndMenu();

//	menuBar->Append(fileMenu, "&File");

//	wxMenu* editMenu = new wxMenu;

	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Welcome to lfant!~");

	// Specifics
//	StartAuiManager();
//	{
	manager = new wxAuiManager(this);
	printf("Starting construction.\n");
		StartAuiNotebook("notebook1");
		{
			Log("Starting panel.");
			StartPanel("panel1", "Inspector");
			{
				Log("Starting box sizer.");
				StartBoxSizer(Orient::Vertical);
				{
					Log("Adding button.");
					AddWidget(new wxButton(GetLastWidget(2), GetId("btn1"), "Click Me!"));
					Log("Closing all widget holes.");
				//	Button("btn1", "Click Me!");
				}
				EndWidget();
			}
			EndWidget();
		}
		EndWidget();

		StartAuiNotebook("notebook2");
		{
			Log("Starting panel.");
		//	StartPanel("panel2", "Viewport");
			{
				Log("Starting box sizer.");
			//	StartBoxSizer(Orient::Vertical);
				{
					Log("Adding button.");
				//	AddWidget(new wxButton(GetLastWidget(2), GetId("btn2"), "Click Me!"));
					canvas = new wxGLCanvas(GetLastWidget(1), GetId("canvas1"));
					context = new wxGLContext(canvas);
					AddWidget(canvas);
					canvas->Show(true);
					canvas->Update();
					manager->Update();
				//	Log(canvas->IsShownOnScreen());
				//	Log(canvas->IsShown(), canvas->IsTopLevel(), canvas->GetParent() == NULL, canvas->GetParent()->IsShownOnScreen());
					
					Log("Closing all widget holes.");
				//	Button("btn1", "Click Me!");
				}
			//	EndWidget();
			}
		//	EndWidget();
		}
		EndWidget();
	manager->Update();
//	}
//	EndWidget();
	Log("Window created.");
}

void Window::OnQuit(wxCommandEvent& event)
{
	Close(true);
	game->Exit();
}

void Window::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("lfant engine editor", "Biography", wxOK, this);
	canvas->SetCurrent(*context);
	game->renderer->Init();
	game->renderer->Update();
	canvas->SwapBuffers();
}

int Window::GetId(string name)
{
	int& id = ids[name];
	if(!id)
	{
		++currId;
		id = currId;
	}
	return id;
}

int Window::AddId(string name)
{
	return ids[name] = ++currId;
}

wxAuiNotebook* Window::AddPane(string name, string label)
{
	wxAuiNotebook* curr = new wxAuiNotebook(this, GetId(name));
	notebooks.push_back(curr);
//	manager->AddPane(curr, wxAuiPaneInfo().Name(label).CenterPane().CloseButton().Floatable().Movable(true));
	return curr;
}

wxAuiNotebook* Window::GetPane(string name)
{
	int& id = ids[name];
	if(!id) return nullptr;
	for(auto& n : notebooks)
	{
		if(n->GetId() == id)
		{
			return n;
		}
	}
	return nullptr;
}

void Window::StartWidget(wxWindow* widget)
{
	AddWidget(widget);
	widgetStack.push_back(widget);
}

void Window::AddWidget(wxWindow* widget)
{
	if(wxSizer* sizer = dynamic_cast<wxSizer*>(GetLastObject()))
	{
		sizer->Add(widget, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	}
	
	if(wxAuiNotebook* nt = dynamic_cast<wxAuiNotebook*>(GetLastObject()))
	{
		nt->AddPage(widget, widget->GetName());
	}
}

void Window::EndWidget()
{
	widgetStack.pop_back();
}

wxWindow* Window::GetLastWidget(int back)
{
	if(widgetStack.size() >= back)
	{
		return dynamic_cast<wxWindow*>(widgetStack[widgetStack.size()-back]);
	}
	return this;
}

wxObject* Window::GetLastObject(int back)
{
	if(widgetStack.size() >= back)
	{
		return widgetStack[widgetStack.size()-back];
	}
	return this;
}

wxAuiNotebook* Window::StartAuiNotebook(string name, string label)
{
	wxAuiNotebook* curr = new wxAuiNotebook(GetLastWidget(), GetId(name));
	notebooks.push_back(curr);
	manager->AddPane(curr, wxAuiPaneInfo().Name(label).CenterPane().CloseButton().Floatable().Movable(true));
	StartWidget(curr);
	return curr;
}

wxPanel* Window::StartPanel(string name, string label)
{
	wxPanel* panel = new wxPanel(GetLastWidget(), GetId(name));
//	if(wxAuiNotebook* nt = dynamic_cast<wxAuiNotebook*>(GetLastObject()))
//	{
//		nt->AddPage(panel, label);
//	}
	panel->SetName(label);
	StartWidget(panel);
	return panel;
}

wxMenu* Window::StartMenu(string label)
{
	wxMenu* menu = new wxMenu;
	menuBar->Append(menu, label);
	menuStack.push_back(menu);
	return menu;
}

void Window::EndMenu()
{
	menuStack.pop_back();
}

void Window::AddMenuItem(string name, string label)
{
	menuStack[menuStack.size()-1]->Append(GetId(name), label);
}

void Window::AddMenuSeparator()
{
	menuStack[menuStack.size()-1]->AppendSeparator();
}

wxBoxSizer* Window::StartBoxSizer(int orient)
{
	wxBoxSizer* sizer = new wxBoxSizer(orient);
	GetLastWidget()->SetSizer(sizer);
	sizer->Fit(GetLastWidget());
	sizer->SetSizeHints(GetLastWidget());

	if(wxSizer* sizerp = dynamic_cast<wxSizer*>(GetLastObject()))
	{
		sizerp->Add(sizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	}
	widgetStack.push_back(sizer);
	return sizer;
}

}
}
}