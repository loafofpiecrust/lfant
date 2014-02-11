
#pragma once

// internal
#include <lfant/Object.h>

// external
#include <wx/scrolwin.h>
#include <wx/dataview.h>
#include <wx/propgrid/propgrid.h>
#include <wx/collpane.h>
#include <wx/control.h>
#include <wx/treectrl.h>

class wxGrid;

namespace lfant {

class Entity;
class Properties;

namespace editor {
namespace gui {

class EntityTree;

class Inspector : public wxControl, public lfant::Object
{
	class Item : public wxCollapsiblePane
	{
	public:
		Item(wxWindow* parent, wxWindowID id = wxID_ANY, string label = "", long style = 0);
		~Item();

		wxPropertyGrid* GetGrid() { return grid; }

		void AddProp(wxPGProperty* parent, Properties* prop);

	protected:
	//	wxCollapsiblePane* pane = nullptr;
		wxPropertyGrid* grid = nullptr;
	};
public:
	Inspector(EntityTree* tree, wxWindow* parent, wxWindowID id = wxID_ANY, long style = wxDV_NO_HEADER|wxDV_ROW_LINES);
	~Inspector();

	virtual void Init();

	void Refresh();
	void Apply();

	Entity* GetEntity();

	Inspector::Item* AppendItem(string name);
	Inspector::Item* GetItem(string name);

	EntityTree* tree = nullptr;
protected:

	void OnReselect(wxTreeEvent& evt);

	void SetEntity(Entity* ent);

	void OnSetValue(wxCommandEvent& evt);

	wxPGProperty* compNode;
	Properties* props = nullptr;
	bool setup = false;
	Entity* entity = nullptr;

	deque<Inspector::Item*> items;
};

} // namespace gui
} // namespace editor
} // namespace lfant

