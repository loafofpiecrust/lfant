
#pragma once

// internal
#include <lfant/Object.h>

// external
#include <wx/treectrl.h>

namespace lfant {

class Entity;

namespace editor {
namespace gui {

class EntityTree : public wxTreeCtrl, public lfant::Object
{
	friend class Inspector;
public:
	EntityTree(wxWindow* parent, wxWindowID id = wxID_ANY, long style = wxTR_DEFAULT_STYLE);
	~EntityTree();

	virtual void Init();

	void AppendItem(const wxTreeItemId& parent, Entity* ent);
	void AppendItem(Entity* entity);

	wxTreeItemId GetItem(string name);
	wxTreeItemId GetItem(Entity* ent);

	Entity* GetCurrent();

protected:
	void UpdateName(Entity* ent);
	void OnSetCurrent(wxTreeEvent& evt);

private:
	wxTreeItemId GetItem(wxTreeItemId parent, Entity* ent, void* iter);
	Entity* current = nullptr;
};

} // namespace gui
} // namespace editor
} // namespace lfant
