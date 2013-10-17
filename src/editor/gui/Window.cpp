
#include <editor/gui/Window.h>

// Internal
#include <editor/Editor.h>
#include <lfant/Console.h>
#include <lfant/Renderer.h>
#include <lfant/Component.h>
#include <lfant/Entity.h>

// External
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/glcanvas.h>
#include <wx/statline.h>
#include <wx/collpane.h>
#include <wx/grid.h>

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
//	Connect(GetId("File.Quit"), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&Window::OnQuit);
//	Connect(GetId("File.About"), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&Window::OnAbout);

	// Basic Setup

	CreateStatusBar();
	SetStatusText("Welcome to lfant!~");

	menuBar = new wxMenuBar;
	SetMenuBar(menuBar);
	StartMenu("&File");
	{
		AddMenuItem("File.NewProject", "New Project");
		AddMenuItem("File.LoadProject", "Load Project...");
		AddMenuItem("File.SaveProject", "Save Project");

		AddMenuSeparator();

		AddMenuItem("File.NewScene", "&New Scene");
		AddMenuItem("File.LoadScene", "Load Scene...");
		AddMenuItem("File.SaveScene", "&Save Scene");

		AddMenuSeparator();

		AddMenuItem("File.About", "About", (wxObjectEventFunction)&Window::OnAbout);
		AddMenuItem("File.Quit", "E&xit", (wxObjectEventFunction)&Window::OnQuit);
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


	// Specifics
//	StartAuiManager();
//	{
	manager = new wxAuiManager(this);
	printf("Starting construction.\n");
		StartAuiNotebook("notebook1", "", wxLEFT);
		{
		//	Log("Starting panel.");
			StartPanel("panel1", "Inspector");
			{
			//	Log("Starting box sizer.");
				StartBoxSizer(Orient::Vertical);
				{
				//	Log("Adding button.");
					AddWidget(new wxButton(GetLastWidget(), GetId("btn1"), "Click Me!"));
				//	Log("Closing all widget holes.");
				//	Button("btn1", "Click Me!");
				}
				EndWidget();
			}
			EndWidget();
		}
		EndWidget();

		auto note = StartAuiNotebook("notebookMid", "Viewport");
		{
			manager->GetPane(note).CenterPane();

			auto win = new wxScrolledWindow(GetLastWidget(), GetId());
			StartWidget(win);
			{
				AddWidget(new wxStaticText(GetLastWidget(), GetId(), "Well hello there."));
			}
			EndWidget();
		}
		EndWidget();

		StartAuiNotebook("notebook2", "EntityStuff", wxRIGHT);
		{
		//	Log("Starting panel.");
		//	StartPanel("panel2", "Viewport");
			auto win = new wxScrolledWindow(GetLastWidget(), GetId());
			StartWidget(win);
			{
				win->SetName("pagi");
			//	Log("Starting box sizer.");
				StartBoxSizer(Orient::Vertical);
				currentEnt = new Entity;
				if(currentEnt)
				{
				//	Log("Adding button.");
					// add a save button
				/*	StartBoxSizer(Orient::Horizontal);
					{
						AddWidget(new wxStaticText(GetLastWidget(), GetId(), "Name"));
						AddWidget(new wxTextCtrl(GetLastWidget(), GetId("Entity.Name"), currentEnt->name, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP));
					}
					EndWidget();*/

					AddWidget(new wxButton(GetLastWidget(), GetId("btn2"), "Click Me!"));
					AddWidget(new wxStaticLine(GetLastWidget(), GetId()));

					// save current entity to a Properties*
					for(auto& comp : currentEnt->components)
					{
						auto grid = new wxGrid(GetLastWidget(), GetId("Entity"+type::Descope(type::Name(comp), "lfant")+".ValueGrid"));
						Properties* prop = new Properties;
						comp->Save(prop);
						grid->CreateGrid(prop->values.size(), 1);
						for(uint i = 0; i < prop->values.size(); ++i)
						{
							grid->SetRowLabelValue(i, prop->values[i].first);
							grid->GetTable()->SetValue(i, 0, prop->values[i].second);
						}
						grid->EnableEditing(true);
						grid->EnableGridLines(true);
						grid->EnableDragColSize(true);
						grid->EnableDragRowSize(false);
						grid->SetColLabelValue(0, "Value");
						grid->SetMargins( 0, 0 );
						grid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
						grid->AutoSizeRows();
						AddWidget(grid, wxEXPAND);

					}

					// set row names as value labels in the entity Properties
					// set values from it as well
					// something like so:
					// 	Connect(grid, wxEVT_VALUE_CHANGED, (wxObjectEventFunction)&Window::SetEntityValue);
					// and:
					//  Connect(saveBtn, wxEVT_COMMAND_CLICKED, (wxObjectEventFunction)&Window::SaveCurrentEntity);

				/*	auto pan = new wxCollapsiblePane(GetLastWidget(), GetId("Viewport.Comp"), "SomeComponent");
					StartWidget(pan, wxEXPAND);
					{
						StartBoxSizer(Orient::Vertical);
						{
							AddWidget(new wxStaticText(pan->GetPane(), GetId(), "Words on words"));

						}
						EndWidget();
					}
					EndWidget();*/
				/*	canvas = new wxGLCanvas(GetLastWidget(1), GetId("canvas1"));
					context = new wxGLContext(canvas);
					AddWidget(canvas);
					canvas->Show(true);
					canvas->Update();
					manager->Update();*/
				//	Log(canvas->IsShownOnScreen());
				//	Log(canvas->IsShown(), canvas->IsTopLevel(), canvas->GetParent() == NULL, canvas->GetParent()->IsShownOnScreen());
					
				//	Log("Closing all widget holes.");
				//	Button("btn1", "Click Me!");
				}
				EndWidget();
			}
			EndWidget();
		}
		EndWidget();
	manager->Update();
	Layout();
//	}
//	EndWidget();
//	Log("Window created.");
}

void Window::OnQuit(wxCommandEvent& event)
{
	SetMenuBar(nullptr);
	menuBar.reset();
	Close(true);
//	game->Exit();
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
	if(name == "") return ++currId;

	int& id = ids[name];
	if(!id)
	{
		id = currId;
		++currId;
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

void Window::StartWidget(wxWindow* widget, int flags)
{
	AddWidget(widget, flags);
	widgetStack.push_back(widget);
}

void Window::AddWidget(wxWindow* widget, int flags)
{
	if(wxSizer* sizer = dynamic_cast<wxSizer*>(GetLastObject()))
	{
		int f = wxALL;
		if(flags != -1)
		{
			f |= flags;
		}
		sizer->Add(widget, 0, f, 5);
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
		if(dynamic_cast<wxSizer*>(widgetStack[widgetStack.size()-back]))
		{
			return GetLastWidget(back+1);
		}

		// recheck stack size
		if(widgetStack.size() >= back)
		{
			return dynamic_cast<wxWindow*>(widgetStack[widgetStack.size()-back]);
		}
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

wxAuiNotebook* Window::StartAuiNotebook(string name, string label, int pos)
{
	wxAuiNotebook* curr = new wxAuiNotebook(GetLastWidget(), GetId(name));
	notebooks.push_back(curr);
	manager->AddPane(curr, pos, label);
	manager->GetPane(curr).Floatable().Movable(true);
//	manager->AddPane(curr, wxAuiPaneInfo().Name(label).CenterPane().Position(pos).CloseButton().Floatable().Movable(true));
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

void Window::AddMenuItem(string name, string label, wxObjectEventFunction evt)
{
	if(evt)
	{
		Connect(GetId(name), wxEVT_COMMAND_MENU_SELECTED, evt);
	}
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