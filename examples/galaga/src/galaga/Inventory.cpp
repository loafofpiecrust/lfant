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

void Inventory::Init()
{
	ConnectEvent(SENDER(owner, UseItem), RECEIVER(this, UseItem));
}

void Inventory::UseItem(byte mode)
{
	equippedItem->Use(mode);
}

void Inventory::AddItem(Item* item)
{
	if(massLimit > 0.0f && mass+item->mass > massLimit)
	{
		return;
	}
	items.push_back(item);
	mass += item->mass;
	item->inventory = this;
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
	for(auto& i : items)
	{
		if(i->owner->name == name)
		{
			i->owner->active = true;
			i->Enable(true);
			i->equipped = true;

			i->owner->transform->SetPosition(i->equippedPosition);
			equippedItem->owner->transform->SetPosition(equippedItem->initialPosition);

			equippedItem = i;
			return;
		}
	}
}


IMPLEMENT_COMP(Item)

Item::Item()
{
}

Item::~Item()
{
}

void Item::Use(byte mode)
{
	TriggerEvent("UseItem", mode);
}

}
}