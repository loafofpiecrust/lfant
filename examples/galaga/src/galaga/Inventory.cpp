/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2013-06-04 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/

#include <galaga/Inventory.h>

// Internal
#include <lfant/Transform.h>
#include <lfant/Console.h>

// External
 
namespace lfant {
namespace galaga {

IMPLEMENT_COMP(Inventory)

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("equippedItem", equippedItem);
	prop->Set("itemLimit", itemLimit);
	prop->Set("massLimit", massLimit);
	prop->Set("mass", mass);
}

void Inventory::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("equippedItem", equippedItem);
	prop->Get("itemLimit", itemLimit);
	prop->Get("massLimit", massLimit);
	prop->Get("mass", mass);
}

void Inventory::Init()
{
	ConnectEvent(SENDER(owner, UseItem), RECEIVER(this, UseItem));
	ConnectEvent(SENDER(owner, EndUseItem), RECEIVER(this, EndUseItem));
	ConnectEvent(SENDER(owner, EquipItem), this, (void(Inventory::*)(uint32_t))&Inventory::Equip);
}

void Inventory::UseItem(byte mode)
{
	Log("Gonna use item");
	items[equippedItem]->Use(mode);
}

void Inventory::EndUseItem()
{
	items[equippedItem]->EndUse();
}

void Inventory::AddItem(Item* item)
{
	Log("Adding item to inventory");
	if((massLimit > 0.0f && mass+item->mass > massLimit) || (itemLimit > 0 && items.size() >= itemLimit))
	{
		return;
	}
	items.push_back(item);
	mass += item->mass;
	item->inventory = this;

	if(items.size() == 1)
	{
		item->Equip(true);
	}
}

void Inventory::RemoveItem(Item* item)
{
	for(uint i = 0; i < items.size(); ++i)
	{
		if(items[i] == item)
		{
			mass -= item->mass;
			items.erase(items.begin()+i);
			return;
		}
	}
}

void Inventory::RemoveItem(string name)
{
	for(uint i = 0; i < items.size(); ++i)
	{
		if(items[i]->owner->name == name)
		{
			mass -= items[i]->mass;
			items.erase(items.begin()+i);
			return;
		}
	}
}

Item* Inventory::GetItem(string name)
{
	for(auto& i : items)
	{
		if(i->owner->name == name)
		{
			return i;
		}
	}
	return nullptr;
}

void Inventory::Equip(string name)
{
	for(uint i = 0; i < items.size(); ++i)
	{
		if(items[i]->owner->name == name)
		{
			Equip(i);
			return;
		}
	}
}

void Inventory::Equip(uint32_t idx)
{
	if(items.size() <= idx || equippedItem == idx)
	{
		return;
	}

	if(items.size() > equippedItem) items[equippedItem]->Equip(false);
	equippedItem = idx;
	items[equippedItem]->Equip(true);
}


IMPLEMENT_COMP(Item)

Item::Item()
{
}

Item::~Item()
{
}

void Item::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("mass", mass);
	prop->Set("initialPosition", initialPosition);
	prop->Set("equippedPosition", equippedPosition);
	prop->Set("equipped", equipped);
}

void Item::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("mass", mass);
	Log("Loading, mass: ", mass);
	prop->Get("initialPosition", initialPosition);
	prop->Get("equippedPosition", equippedPosition);
	Log("Loading, equippedPos: ", lexical_cast<string>(equippedPosition));
	prop->Get("equipped", equipped);
}

void Item::Init()
{
	Inventory* inv = nullptr;
	if(owner->parent && (inv = owner->parent->GetComponent<Inventory>()))
	{
		Log("Adding item to parent inventory");
		inv->AddItem(this);
	}
}

void Item::Equip(bool val)
{
	Log("Activating this item");
	owner->active = val;
//	Enable(val);
	Log("Setting equipped");
	equipped = val;
	if(val)
	{
		Log("Setting position, equippedPos: ", lexical_cast<string>(equippedPosition));
		owner->transform->SetPosition(equippedPosition);
	}
	else
	{
		owner->transform->SetPosition(initialPosition);
	}
	Log("Done");
}

void Item::Use(byte mode)
{
	Log("ITEM USED!");
	TriggerEvent("Use", mode);
}

void Item::EndUse()
{
	TriggerEvent("EndUse");
}

}
}