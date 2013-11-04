
#include <editor/gui/Window.h>

// Internal
#include <editor/Editor.h>
#include <lfant/Console.h>
#include <lfant/Renderer.h>
#include <lfant/Component.h>
#include <lfant/Entity.h>
#include <lfant/Scene.h>
#include <editor/gui/GLCanvas.h>
#include <editor/gui/ValueGrid.h>

// External
#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/glcanvas.h>
#include <wx/statline.h>
#include <wx/collpane.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/treebase.h>
#include <wx/event.h>

namespace lfant {
namespace editor {
namespace gui {

//BEGIN_EVENT_TABLE(Window, wxFrame)
//	EVT_BUTTON(Window::SaveCurrentEntity)
//END_EVENT_TABLE()

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

	StartMenu("Scene");
	{
		AddMenuItem("Scene.AddEnt", "Add Entity");

		wxFrame::Bind(wxEVT_COMMAND_MENU_SELECTED, &Window::AddEntity, this, GetId("Scene.AddEnt"));
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
		StartPanel("panel1", "Inspector");
		{
		//	Log("Starting box sizer.");
			StartBoxSizer(Orient::Vertical);
			{
			//	Log("Adding button.");
			//	AddWidget(new wxButton(GetLastWidget(), GetId("btn1"), "Click Me!"));
				tree = new wxTreeCtrl(GetLastWidget(), GetId("SceneTree"));
				AddWidget(tree, wxEXPAND, 1);

				auto root = tree->AddRoot("scene");
			}
			EndWidget();
		}
		EndWidget();
	}
	EndWidget();

	StartAuiNotebook("notebook2", "EntityStuff", wxRIGHT);
	{
	//	Log("Starting panel.");
	//	StartPanel("panel2", "Viewport");
		auto win = new wxScrolledWindow(GetLastWidget(), GetId("GridContainer"));
		StartWidget(win);
		{
			win->SetName("pagi");
		//	Log("Starting box sizer.");
		//	StartBoxSizer(Orient::Vertical);
			{
		//		AddWidget(new wxButton(GetLastWidget(), GetId("Entity.SaveButton"), "Save"));


				gridContSizer = StartBoxSizer(Orient::Vertical);
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

	Object::ConnectEvent(SENDER(game->scene, AddEntity), RECEIVER(this, FindAddEntityNode));
	tree->Bind(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, &Window::SetCurrentEntity, this, tree->GetId());
}

void Window::AddEntity(wxCommandEvent& evt)
{
	game->scene->Spawn("Entity", currentEnt);
}

void Window::AddEntityNode(wxTreeItemId& parent, Entity* entity)
{
	auto id = tree->AppendItem(parent, entity->name, -1, -1, (wxTreeItemData*)entity);
	tree->SetItemHasChildren(id, true);
	for(auto& child : entity->children)
	{
		AddEntityNode(id, child);
	}
}

void Window::FindAddEntityNode(Entity* entity)
{
	wxTreeItemId id = tree->GetRootItem();
	if(!entity->parent)
	{
		AddEntityNode(id, entity);
	}
	else
	{
		void* vali = 0;
		auto item = FindEntityInTree(id, entity->parent, vali);
		if(item.IsOk())
		{
			AddEntityNode(item, entity);
		}
	}
}

wxTreeItemId Window::FindEntityInTree(wxTreeItemId parent, Entity* entity, void* vali)
{
	auto item = tree->GetFirstChild(parent, vali);
	uint childCount = tree->GetChildrenCount(parent, false);
	for(uint i = 0; i < childCount; ++i)
	{
		if(!item.IsOk()) break;

		if((Entity*)tree->GetItemData(item) == entity)
		{
			return item;
		}

		void* vali2 = 0;
		wxTreeItemId temp = FindEntityInTree(item, entity, vali2);
		if(temp != item)
		{
			return temp;
		}

		item = tree->GetNextChild(parent, vali);
	}
	return parent;
}

void Window::ClearTree(wxTreeItemId id)
{
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

void Window::SaveCurrentEntity(wxCommandEvent& evt)
{
//	Log("Window::SaveCurrentEntity: Touch.");
	if(!currentProp) { Log("Window::Save: invalid props"); return; }

	Log("Window::Save: Touch.");
	Log("Window::Save: prop ", currentProp);
//	wxSizerItemList& sizerItems = gridContSizer->GetChildren();
	for(uint i = 0; i < compGrids.size(); ++i)
	{
		auto grid = compGrids[i];
		grid->ForceRefresh();

		Log("Window::Save: prop child ", i, ": ", currentProp->children[i].get());
		for(uint k = 0; k < grid->GetNumberRows(); ++k)
		{
			Log("Setting value '"+grid->GetCellValue(k, 0).ToStdString()+"' to '"+grid->GetCellValue(k, 1).ToStdString()+"'.");
			currentProp->children[i]->Set(grid->GetCellValue(k, 0).ToStdString(), grid->GetCellValue(k, 1).ToStdString());
		}
	}
	currentEnt->Load(currentProp);
}

wxWindow* Window::GetWidget(string name)
{
	return wxWindow::FindWindowById(GetId(name));
}

void Window::SetCurrentEntity(wxTreeEvent& evt)
{
	Log("Window::SetCurrentEntity: Touch.");

	currentEnt = (Entity*)tree->GetItemData(evt.GetItem());
	gridContSizer->Clear(true);
	wxSizerItemList& sizerItems = gridContSizer->GetChildren();
//	currentProp->children.clear();
	if(currentProp) delete currentProp;
	currentProp = new Properties();
	Log("Change entity, new props: ", currentProp, "; vals: ", &(currentProp->values));
	currentEnt->Save(currentProp);

	compGrids.clear();

	auto saveBtn = new wxButton(gridContSizer->GetContainingWindow(), GetId("Entity.SaveButton"), "Save");
	AddWidget((wxWindow*)gridContSizer, saveBtn);
	/*GetWidget("Entity.SaveButton")->*/saveBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Window::SaveCurrentEntity, this, saveBtn->GetId());

	for(uint i = 0; i < currentEnt->components.size(); ++i)
	{
	//	Log("Window::SetCurrentEntity: In the loop");
		Component* comp = currentEnt->components[i];
	//	wxSizerItem* item = nullptr;
	//	if(i < sizerItems.size()) item = sizerItems[i];

	//	Properties* prop = new Properties;
	//	comp->Save(prop);
	//	currentProp->AddChild(prop);
		Properties* prop = currentProp->children[i];

		string compName = type::Descope(type::Name(comp), "lfant");
		auto coll = new wxCollapsiblePane(gridContSizer->GetContainingWindow(), GetId("Entity."+compName+".Pane"), compName);
		gridContSizer->Add(coll, 0, wxALL|wxEXPAND, 5);

		auto collSizer = new wxBoxSizer(wxVERTICAL);
		auto grid = new ValueGrid(coll->GetPane(), GetId("Entity."+compName+".ValueGrid"));
		collSizer->Add(grid, 1, wxALL|wxGROW|wxEXPAND, 2);
		coll->GetPane()->SetSizer(collSizer);
		collSizer->SetSizeHints(coll->GetPane());

		grid->CreateGrid(prop->values.size(), 2);
		for(uint i = 0; i < prop->values.size(); ++i)
		{
			grid->SetCellValue(i, 0, prop->values[i].first);
			grid->SetReadOnly(i, 0);
			grid->SetCellValue(i, 1, prop->values[i].second);
		}
		grid->EnableEditing(true);
		grid->EnableGridLines(true);
		grid->EnableDragColSize(false);
		grid->EnableDragRowSize(false);
		grid->HideColLabels();
		grid->HideRowLabels();
		grid->SetMargins( 0, 0 );
		grid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
		grid->AutoSizeRows();

		Log("Grid ", i, " has default editor '", type::Name(grid->GetDefaultEditor()));

		compGrids.push_back(grid);

	/*	if(item)
		{
			gridContSizer->Replace(item->GetWindow(), grid);
			item->GetWindow()->Destroy();
		}
		else*/
		{
		}
	//	AddWidget((wxWindow*)gridContSizer, grid, wxEXPAND);
	}
	gridContSizer->Layout();
//	gridContSizer->Fit(wxWindow::FindWindowById(GetId("GridContainer")));
//	gridContSizer->GetContainingWindow()->Update();
//	Layout();
}

void Window::Render()
{
	canvas->Paint();
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
		sizerp->Add(sizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	}
	else
	{
		GetLastWidget()->SetSizer(sizer);
		sizer->Fit(GetLastWidget());
		sizer->SetSizeHints(GetLastWidget());
	}
	widgetStack.push_back(sizer);
	return sizer;
}

}
}
}