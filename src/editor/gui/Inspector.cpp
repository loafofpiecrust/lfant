#include "Inspector.h"

// internal
#include <lfant/Properties.h>
#include <lfant/Entity.h>
#include <lfant/Console.h>

#include <editor/gui/EntityTree.h>
#include <editor/gui/Window.h>
#include <editor/gui/ValueGrid.h>

// external
#include <wx/grid.h>
#include <wx/collpane.h>
#include <wx/button.h>
#include <wx/sizer.h>

namespace lfant {
namespace editor {
namespace gui {

Inspector::Item::Item(wxWindow* parent, wxWindowID id, string label, long style) :
	wxCollapsiblePane(parent, id, label)
{
//	SetName(label);
	GetPane()->SetSizer(new wxBoxSizer(wxVERTICAL));
	grid = new wxPropertyGrid(GetPane(), Window::GetId(label+".Grid"));

	wxSizer* sizer = GetPane()->GetSizer();

	sizer->Add(grid, 1, wxALL|wxEXPAND|wxGROW);
	sizer->SetSizeHints(GetPane());
	sizer->SetMinSize(50, 50);

	GetPane()->Fit();
	GetPane()->SetSize({100,150});

	grid->FitInside();
}

Inspector::Item::~Item()
{
}

Inspector::Inspector(EntityTree* tree, wxWindow* parent, wxWindowID id, long style) :
	wxControl(parent, id, wxDefaultPosition, wxDefaultSize, style),
	tree{tree},
	props{new Properties}
{
	SetSizer(new wxBoxSizer(wxVERTICAL));
}

Inspector::~Inspector()
{
	std::cout << ("\n\tDestroying\n\n");
}

void Inspector::Init()
{
	Object::Init();

	wxWindow::Bind(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, &Inspector::OnReselect, this, tree->GetId());

//	ConnectEvent(tree, "SetCurrent", this, &Inspector::Refresh);
	ConnectEvent((Object*)tree, "SetCurrent", this, &Inspector::Refresh);
	Log("Inspector tree ref: ", tree);
}

Entity* Inspector::GetEntity()
{
	return entity;
}

Inspector::Item* Inspector::AppendItem(string name)
{
	Item* item = new Item(this, Window::GetId("Entity."+name), name);
	if(GetSizer())
		GetSizer()->Add(item, 0, wxALL|wxEXPAND|wxGROW);
	items.push_back(item);
	return item;
}

Inspector::Item* Inspector::GetItem(string name)
{
	for(uint i = 0; i < items.size(); ++i)
	{
		Item* item = items[i];
		if(item->GetName() == name)
		{
			return item;
		}
	}
}

void Inspector::OnReselect(wxTreeEvent& evt)
{
	Refresh();
}

void Inspector::SetEntity(Entity* ent)
{
	entity = ent;
}

void Inspector::OnSetValue(wxCommandEvent& evt)
{
	wxTextCtrl* win = (wxTextCtrl*)evt.GetEventObject();
	if(win->GetName() == "name")
	{
		GetEntity()->Rename(win->GetLineText(0));
		tree->UpdateName(GetEntity());
	}
	else
	{
		props->Set(win->GetName(), (string)win->GetLineText(0));
	}
}

void Inspector::Item::AddProp(wxPGProperty* parent, Properties* prop)
{
	Log("Inspector:AddPropToGrid: Touch");
	string compName = prop->name;
	if(compName.empty()) compName = prop->type;

	Log("Inspector:AddPropToGrid: Appending container '"+compName+"'");
//	auto win = AppendContainer(parent, compName);

	for(auto& val : prop->values)
	{
	//	Log("Inspector:AddPropToGrid: Appending value '"+val.first+"'");
	//	AppendItem(win, val.first, NO_IMAGE, (wxClientData*)&(val.second));
		wxStringProperty* prop = new wxStringProperty(val.first, compName+"."+val.first, val.second);
		prop->SetClientData((void*)&(val.second));

		if(parent != grid->GetRoot())
			grid->AppendIn(parent, prop);
		else
			grid->Append(prop);
	}

	for(auto& child : prop->children)
	{
		if(child->type == "Component")
		{
			wxPGProperty* pgp = new wxPGProperty(child->name, compName+"."+child->name);
			GetGrid()->AppendIn(parent, pgp);

			AddProp(pgp, prop->GetChild(child->name));
		}
	}
}

void Inspector::Refresh()
{
	Log("Inspector:Refresh: Touch.");
	if(!tree->GetCurrent())
	{
		return;
	}
	Log("Inspector:Refresh: Past tree check");

	SetEntity(tree->GetCurrent());

	props->Clear();

	Log("Change entity, new props: ", props, "; vals: ", &(props->values));
	GetEntity()->Save(props);

	Log("Inspector:Refresh: Entity saved");
	// New method

	///@todo Add buttons for 'Refresh' and 'Apply' here.

	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxVERTICAL);
		GetSizer()->Add(btnSizer);

		btnSizer->Add(new wxButton(this, Window::GetId("Entity.Refresh"), "Refresh"));
		btnSizer->Add(new wxButton(this, Window::GetId("Entity.Apply"), "Apply"));

		btnSizer->Layout();
	}

	// Clear all items
	Log("Inspector:Refresh: clearing items");
//	GetStore()->DeleteAllItems();
	GetSizer()->Clear();

	// Basic entity properties
	Log("Inspector:Refresh: appending entity vars to ");
//	AppendItem(wxDataViewItem(0), "name");
	wxPropertyGrid* grid = AppendItem("Entity")->GetGrid();

/*	for(auto& val : props->GetRoot())
	{
		string first = val.name;
		string second = (string)val.element.CastTo<json::String>();

		wxStringProperty* prop = new wxStringProperty(first, "Entity."+first, second);
		prop->SetClientData((void*)&(val));
		grid->Append(prop);
	}*/

	GetSizer()->AddSpacer(12);

	// Container to hold components
	Log("Inspector:Refresh: appending component container");
//	compNode = AppendContainer(wxDataViewItem(0), "Components");

//	deque<Properties*> comps = props->GetChildren("component");
	for(Properties* comp : *props)
	{
		if(comp->type != "component") continue;

		Item* item = AppendItem(comp->Get("type"));
		item->AddProp(item->GetGrid()->GetRoot(), comp);
		GetSizer()->AddSpacer(12);
	}

	Layout();
}

void Inspector::Apply()
{
	if(!props)
	{
		Log("Window::Save: invalid props");
		return;
	}

	wxPropertyGrid* grid = GetItem("Entity")->GetGrid();
	wxPropertyGridIterator it;
	for(it = grid->GetIterator(); !it.AtEnd(); it++)
	{
		wxPGProperty* prop = *it;
		props->Set(prop->GetName(), Window::ConvertString(prop->GetValueAsString()));
	}

	for(uint i = 1; i < items.size(); ++i)
	{
		grid = items[i]->GetGrid();

		uint k = 0;
		for(it = grid->GetIterator(); !it.AtEnd(); it++, ++k)
		{
			wxPGProperty* prop = *it;
			props->children[k]->Set(prop->GetName(), Window::ConvertString(prop->GetValueAsString()));
		}
	}

	GetEntity()->Load(props);
}

} // namespace gui
} // namespace editor
} // namespace lfant
