#include "EntityTree.h"

// internal
#include <lfant/Entity.h>
#include <lfant/Game.h>
#include <lfant/Scene.h>
#include <lfant/Console.h>
#include <editor/gui/Window.h>

// external

namespace lfant {
namespace editor {
namespace gui {

EntityTree::EntityTree(wxWindow* parent, wxWindowID id, long style) :
	wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, style)
{
}

EntityTree::~EntityTree()
{
}

void EntityTree::Init()
{
	AddRoot("scene");

	wxTreeCtrl::Bind(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, &EntityTree::OnSetCurrent, this, GetId());

	Object::ConnectEvent((Object*)game->scene, "AddEntity", this, (void(EntityTree::*)(Entity*))&EntityTree::AppendItem);
}

void EntityTree::AppendItem(const wxTreeItemId& parent, Entity* ent)
{
	auto id = wxTreeCtrl::AppendItem(parent, ent->name, -1, -1, (wxTreeItemData*)ent);
	if(ent->children.size() > 0) SetItemHasChildren(id, true);

	for(auto& child : ent->children)
	{
		AppendItem(id, child);
	}
}

void EntityTree::AppendItem(Entity* entity)
{
	wxTreeItemId id = GetRootItem();
	if(!entity->GetParent())
	{
		AppendItem(id, entity);
	}
	else
	{
		void* vali = 0;
		auto item = GetItem(id, entity->GetParent(), vali);
		if(item.IsOk())
		{
			AppendItem(item, entity);
		}
	}
}

wxTreeItemId EntityTree::GetItem(string name)
{
	return GetItem(game->scene->GetRoot()->GetChild(name, true));
}

wxTreeItemId EntityTree::GetItem(Entity *ent)
{
	void* iter = 0;
	return GetItem(GetRootItem(), ent, iter);
}

wxTreeItemId EntityTree::GetItem(wxTreeItemId parent, Entity* ent, void* iter)
{
	auto item = GetFirstChild(parent, iter);
	uint childCount = GetChildrenCount(parent, false);
	for(uint i = 0; i < childCount; ++i)
	{
		if(!item.IsOk()) break;
		Entity* data = (Entity*)GetItemData(item);

		if(data == ent)
		{
			return item;
		}

		void* iter2 = 0;
		auto temp = GetItem(item, data, iter2);
		if(temp.IsOk())
		{
			return temp;
		}

		item = GetNextChild(parent, iter);
	}
	return nullptr;
}

Entity* EntityTree::GetCurrent()
{
	return current;

	if(GetSelection().IsOk())
	{
		return (Entity*)GetItemData(GetSelection());
	}
	return nullptr;
}

void EntityTree::UpdateName(Entity* ent)
{
	SetItemText(GetItem(ent), ent->name);
}

void EntityTree::OnSetCurrent(wxTreeEvent& evt)
{
	Log("EntityTree::OnSetCurrent: Touch");
	if(evt.GetItem() == GetRootItem())
	{
		return;
	}

	Log("EntityTree::OnSetCurrent: Past root check");

	current = (Entity*)GetItemData(evt.GetItem());
	TriggerEvent("SetCurrent");
}


} // namespace gui
} // namespace editor
} // namespace lfant
