
#include <editor/gui/Window.h>

// Internal
#include <editor/Editor.h>
#include <lfant/Console.h>
#include <lfant/Renderer.h>
#include <lfant/Component.h>
#include <lfant/Entity.h>
#include <lfant/Scene.h>
#include <lfant/Game.h>
#include <lfant/FileSystem.h>

#include <editor/gui/GLCanvas.h>
#include <editor/gui/ValueGrid.h>
#include <editor/gui/DirTree.h>
#include <editor/gui/Inspector.h>
#include <editor/gui/EntityTree.h>

// External
#include <string.h>
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/glcanvas.h>
#include <wx/statline.h>
#include <wx/collpane.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/treebase.h>
#include <wx/event.h>
#include <wx/dirctrl.h>
#include <wx/dirdlg.h>

#include <boost/extension/shared_library.hpp>
#include <boost/filesystem.hpp>

namespace lfant {
namespace editor {
namespace gui {

//BEGIN_EVENT_TABLE(Window, wxFrame)
//	EVT_BUTTON(Window::SaveCurrentEntity)
//END_EVENT_TABLE()

int Window::currId = 1;
qumap<string, int> Window::ids;

Window::Window(string title, Rect rect) :
	wxFrame(0, -1, wxString(title), wxPoint(rect.x, rect.y), wxSize(rect.width, rect.height))
{
	m_isBeingDeleted = true;
//	currentProp = new Properties;
}

Window::~Window()
{
	ClearTree();
}

void Window::Init()
{
//	Connect(GetId("File.Quit"), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&Window::OnQuit);
//	Connect(GetId("File.About"), wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&Window::OnAbout);

	SetMinSize(wxSize(50, 50));
	SetMinClientSize(wxSize(50, 50));

	// Basic Setup

	CreateStatusBar();
	SetStatusText("Welcome to lfant!~");

	if(menuBar) menuBar->DestroyChildren();
	else
	{
		menuBar = new wxMenuBar;
		SetMenuBar(menuBar);
		StartMenu("&File");
		{
			AddMenuItem("File.NewProject", "New Project");
			AddMenuItem("File.LoadProject", "Load Project...", (wxObjectEventFunction)&Window::OnLoadProject);
			AddMenuItem("File.SaveProject", "Save Project");

			AddMenuSeparator();

			AddMenuItem("File.NewScene", "&New Scene");
			AddMenuItem("File.LoadScene", "Load Scene...");
			AddMenuItem("File.SaveScene", "&Save Scene", (wxObjectEventFunction)&Window::SaveScene);

			AddMenuSeparator();

			AddMenuItem("File.About", "About", (wxObjectEventFunction)&Window::OnAbout);
			AddMenuItem("File.Quit", "E&xit", (wxObjectEventFunction)&Window::OnQuit);
		}
		EndMenu();

		if(!game)
		{
			Layout();
			return;
		}
	}

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

	StartMenu("Scene");
	{
		AddMenuItem("Scene.AddEnt", "Add Entity");

		wxFrame::Bind(wxEVT_COMMAND_MENU_SELECTED, &Window::AddEntity, this, GetId("Scene.AddEnt"));
	}
	EndMenu();

	StartMenu("Entity");
	{
		AddMenuItem("Entity.Refresh", "Refresh", (wxObjectEventFunction)&Window::RefreshEntity);
		AddMenuItem("Entity.AddComp", "Add Component", (wxObjectEventFunction)&Window::AddEntityComponent);
	}
	EndMenu();


/*	StartBoxSizer(Orient::Vertical);
	{
		canvas = new GLCanvas((wxFrame*)GetLastWidget(), GetId("canvas1"));
		AddWidget(canvas, wxEXPAND, 1);
		canvas->Init();
	}
	EndWidget();
	Layout();
	return;*/

	// Specifics
//	StartAuiManager();
//	{
	manager = new wxAuiManager(this);
	printf("Starting construction.\n");

	auto note = StartAuiNotebook("notebookMid", "Viewport");
	{
		manager->GetPane(note).CenterPane();

		auto win = new wxScrolledWindow(GetLastWidget(), GetId());
		StartWidget(win);
	//	StartPanel("vp", "Viewport");
		StartBoxSizer(Orient::Vertical);
		{
		//	AddWidget(new wxStaticText(GetLastWidget(), GetId(), "Well hello there."));
			canvas = new GLCanvas((wxFrame*)GetLastWidget(), GetId("canvas1"));
		//	context = new wxGLContext(canvas);
			AddWidget(canvas, wxEXPAND, 1);
			canvas->Init();
		//	canvas->SetCurrent(*context);
		//	canvas->Show(true);
		//	canvas->Update();
		}
		EndWidget();
		EndWidget();
	}
	EndWidget();

	StartAuiNotebook("notebook1", "", wxLEFT);
	{
	//	Log("Starting panel.");
		StartPanel("panel1", "EntityTreePanel");
		{
			GetLastWidget()->SetSize(50, 0);
		//	Log("Starting box sizer.");
			StartBoxSizer(Orient::Vertical);
			{
			//	Log("Adding button.");
			//	AddWidget(new wxButton(GetLastWidget(), GetId("btn1"), "Click Me!"));
				tree = new EntityTree(GetLastWidget(), GetId("SceneTree"));
				AddWidget(tree, wxEXPAND, 1);
				tree->Init();
			//	auto root = tree->AddRoot("scene");
			}
			EndWidget();
		}
		EndWidget();
	}
	EndWidget();

	StartAuiNotebook("notebook2", "EntityStuff", wxRIGHT);
	{
		StartPanel("panel2", "Entsstufs");
		{
			GetLastWidget()->SetSize(50, 0);
			StartBoxSizer(Orient::Vertical);
			{
				inspector = new Inspector(tree, GetLastWidget(), GetId("Inspectorzz"));
				AddWidget(inspector, wxEXPAND, 1);
				inspector->Init();
			}
			EndWidget();
		}
		EndWidget();
	}
	EndWidget();


	StartAuiNotebook("notebook3", "ProjectFiles", wxBOTTOM);
	{
	//	Log("Starting panel.");
	//	StartPanel("panel2", "Viewport");
		auto win = new wxScrolledWindow(GetLastWidget(), GetId("ProjectContainer"));
		StartWidget(win);
		{
			win->SetName("pagi");
		//	Log("Starting box sizer.");
		//	StartBoxSizer(Orient::Vertical);
			{
		//		AddWidget(new wxButton(GetLastWidget(), GetId("Entity.SaveButton"), "Save"));
				StartBoxSizer(Orient::Vertical);
				{
					auto dirTree = new DirTree(GetLastWidget(), GetId("ProjectTree"), "/home/taylorsnead/piecrust/lfant/assets");
					AddWidget(dirTree, wxEXPAND, 1);
				}
				EndWidget();
			}
		//	EndWidget();
		}
		EndWidget();
	}
	EndWidget();

	manager->Update();
	Layout();
//	}
//	EndWidget();
//	Log("Window created.");

//	tree->Bind(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, &Window::SetCurrentEntity, this, tree->GetId());
}

void Window::Deinit()
{
	DestroyChildren();
}

void Window::AddEntity(wxCommandEvent& evt)
{
	game->scene->Spawn(inspector->GetEntity(), "Entity");
}

string Window::ConvertString(const wxString& str)
{
	string result = "";
	for(uint i = 0; i < str.size(); ++i)
	{
		result.push_back((char)str[i].GetValue());
	}
	return result;
}

void Window::ClearTree(wxTreeItemId id)
{
	if(!tree) return;
	if(!id) id = tree->GetRootItem();

	tree->SetItemData(id, nullptr);

	uint childCount = tree->GetChildrenCount(id, false);
	if(childCount <= 0) return;

	void* vali = 0;
	auto item = tree->GetFirstChild(id, vali);
	for(uint i = 0; i < childCount; ++i)
	{
		if(!item.IsOk()) break;

		ClearTree(item);

		item = tree->GetNextChild(id, vali);
	}
}

wxWindow* Window::GetWidget(string name)
{
	return wxWindow::FindWindowById(GetId(name));
}

void Window::Render()
{
	if(canvas) canvas->Paint();
}

void Window::OnQuit(wxCommandEvent& event)
{
//	SetMenuBar(nullptr);
//	menuBar.reset();
	Close(true);
//	game->Exit();
}

void Window::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("lfant engine editor", "Biography", wxOK, this);
//	canvas->SetCurrent(*context);
//	game->renderer->Init();
//	game->renderer->Update();
//	canvas->SwapBuffers();
}

void Window::AddEntityComponent(wxCommandEvent& evt)
{
	wxTextEntryDialog dialog(nullptr, "Add Component");
	if(dialog.ShowModal() == wxID_CANCEL) return;

	string compType = ConvertString(dialog.GetValue());

	if(inspector->GetEntity())
		inspector->GetEntity()->AddComponent(compType);
}

void Window::RefreshEntity(wxCommandEvent& evt)
{
	inspector->tree = tree;
	inspector->Refresh();
}

void Window::OnLoadProject(wxCommandEvent& event)
{
	wxFileDialog dialog(nullptr, "Choose project dir", "", "", "Shared Library (*.so)|*.so", wxFD_OPEN /*| wxFD_CHANGE_DIR*/ | wxFD_FILE_MUST_EXIST);

	if(dialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
//	wxTextEntryDialog dialog(nullptr, "Project Path");
//	wxString str = wxGetTextFromUser("Project Path");
	wxString str = dialog.GetPath();
	string projectPath = "";
	for(int i = 0; i < str.size(); ++i)
	{
	//	cout << "char " << i << ": " << (char)str.at(i).GetValue() << "\n";
		projectPath.push_back((char)str.at(i).GetValue());
	}

//	cout << "Trying to load path:" << dialog.GetPath() << ", dir:" << dialog.GetDirectory() << ", file:" << dialog.GetFilename() << "\n";
	cout << "Trying to load: '" << projectPath << "'\n";

	boost::extensions::shared_library lib(projectPath);
	if(!lib.open())
	{
		cout << "Failed to load " << projectPath << ": '" << dlerror() << "\n";
		return;
	}
	boost::function<void()> func = lib.get<void>("ResetInstance");
	if(func)
	{
		cout << "Loading game...\n";
		func();
		game->standAlone = false;

		boost::filesystem::path p(projectPath);
		game->fileSystem->gameFolder = p.remove_filename().string()+"/../../..";
		game->fileSystem->programFolder = p.remove_filename().string();

		Deinit();
		Init();
	}
	else
	{
		cout << "Couldn't launch game: " << dlerror() << "\n";
	}
}

void Window::SaveScene(wxCommandEvent& event)
{
	game->scene->SaveFile(game->scene->currentFile);
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
	AddWidget(widget, flags, 1);
	widgetStack.push_back(widget);
}

void Window::AddWidget(wxWindow* widget, int flags, int proportion)
{
	if(wxSizer* sizer = dynamic_cast<wxSizer*>(GetLastObject()))
	{
		int f = wxALL;
		if(flags != -1)
		{
			f |= flags;
		}
		sizer->Add(widget, proportion, f, 5);
	}

	if(wxAuiNotebook* nt = dynamic_cast<wxAuiNotebook*>(GetLastObject()))
	{
		nt->AddPage(widget, widget->GetName());
	}
}

void Window::AddWidget(wxWindow* parent, wxWindow* widget, int flags, int proportion)
{
	if(wxSizer* sizer = dynamic_cast<wxSizer*>(parent))
	{
		int f = wxALL;
		if(flags != -1)
		{
			f |= flags;
		}
		sizer->Add(widget, proportion, f, 5);
	}

	if(wxAuiNotebook* nt = dynamic_cast<wxAuiNotebook*>(parent))
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

	if(wxSizer* sizerp = dynamic_cast<wxSizer*>(GetLastObject()))
	{
		sizerp->Add(sizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
	}
	else
	{
		GetLastWidget()->SetSizer(sizer);
	//	sizer->Fit(GetLastWidget());
		sizer->SetSizeHints(GetLastWidget());
	}
	widgetStack.push_back(sizer);
	return sizer;
}

}
}
}
