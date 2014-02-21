/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Entity.h>

// Internal
#include <lfant/Time.h>

#include <lfant/Game.h>
#include <lfant/Scene.h>
#include <lfant/TypeInfo.h>

#include <lfant/Console.h>
#include <lfant/util/String.h>
#include <lfant/Transform.h>

#include <lfant/Component.h>
#include <lfant/ScriptSystem.h>
#include <lfant/Camera.h>
#include <lfant/Random.h>

// External
#include <stdarg.h>

namespace lfant {

bool EntityActive(Entity* ent)
{
	return ent->active;
}

Entity::Entity()
{
	transform = AddComponent<Transform>();
}

Entity::Entity(const Entity& ent)
{
	ent.Clone(this);
}

Entity::~Entity()
{
}

Entity& Entity::operator=(const Entity& other)
{
	other.Clone(this);
	return *this;
}

Entity* Entity::Clone(string name, Entity* parent) const
{
	Entity* ent = new Entity;
	Clone(ent, name, parent);
	return ent;
}

void Entity::Clone(Entity* ent, string name, Entity* parent) const
{
	if(name == "") name = this->name;
	if(!parent) parent = this->parent;

	ent->name = name;
	ent->parent = parent;
	game->scene->GetRoot()->AddChild(ent);

	for(auto& c : components)
	{
		c->Clone(ent);
	}
	for(auto& c : children)
	{
		c->Clone(c->name, ent);
	}
}

uint32_t GenerateId()
{
}

/*******************************************************************************
*
*		Generic Entity functions
*
*******************************************************************************/

void Entity::Init()
{
	Log("Entity::Init: Touch.");
	Object::Init();

//	if(!transform)
//	{
	//	transform = AddComponent(new Transform);
//	}

	if(lifetime <= 0.0f)
	{
		useLifeTime = false;
	}

	if(id == 0)
	{
		// Generate new id?
		id = game->scene->GenerateEntityId();
	}
}

void Entity::Update()
{
	if(lifetime > 0.0f)
	{
		useLifeTime = true;
	}
	if(useLifeTime)
	{
		lifetime -= game->time->deltaTime;
		if(lifetime <= 0.0f)
		{
			Destroy();
		}
	}

	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->Update();
		}
	}
	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->PostUpdate();
		}
	}

	for(auto& child : children)
	{
		if(child->active)
		{
			child->Update();
		}
	}
}

void Entity::Render()
{
	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->Render();
		}
	}

	for(auto& child : children)
	{
		if(child->active)
		{
			child->Render();
		}
	}
}

void Entity::FixedUpdate()
{
	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->FixedUpdate();
		}
	}
	for(auto& child : children)
	{
		if(child->active)
		{
			child->FixedUpdate();
		}
	}
}

void Entity::AddChild(Entity* ent)
{
	children.push_back(ent);
}

Entity* Entity::AddChild()
{
	Entity* ent = new Entity;
	AddChild(ent);
	return ent;
}

void Entity::RemoveChild(Entity* ent)
{
	RemoveChild(ent, true);
}

void Entity::RemoveChild(Entity* ent, bool destroy)
{
	//	children.remove(ptr<Entity>(ent));
	for(uint i = 0; i < children.size(); ++i)
	{
		if(children[i] == ent)
		{
			if(!destroy) children[i] = nullptr;
			children.erase(children.begin()+i);
			return;
		}
	}

}

void Entity::AddComponent(Component* comp)
{
	// @note Needed to call comp->Deinit() here?
	if(comp->GetOwner())
	{
		comp->GetOwner()->RemoveComponent(comp);
	}
	components.push_back(comp);
	comp->owner = this;
	comp->Init();
	TriggerEvent("AddComponent", comp);
	Log("Entity::AddComponent: Calling ", "SetComponent"+type::Descope(type::Name(comp)));
	TriggerEvent("OnSetComponent"+type::Descope(type::Name(comp)), comp);
}

void Entity::Deinit()
{
	Object::Deinit();

	Log("Entity::Destroy: Destroying ", components.size()," components.");
	for(auto& compo : components)
	{
		compo->Deinit();
	}
//	components.clear();

	Log("Entity::Destroy: Destroying ", children.size()," children.");
	for(auto& child : children)
	{
		child->Deinit();
	}
}

void Entity::Destroy()
{
	Log("Entity::Destroy: Touch.");
	Object::Destroy();
	Log("Entity::Destroy: super called.");

	Log("Entity::Destroy: Removing from parent.");
	parent->RemoveChild(this);
}

Component* Entity::GetComponent(string name)
{
	for(auto& comp : components)
	{
		string type = type::Name(comp);
		if(type::Descope(type, "lfant") == name || type::Descope(type) == name || type == name)
		{
			return comp;
		}
		auto reg = Component::typeRegistry.Get(name);
		if(reg && reg->Inherits(type))
		{
			return comp;
		}
	}
	return nullptr;
}

uint32_t Entity::GetLayer() const
{
	if(layer == -1 && parent)
	{
		return parent->GetLayer();
	}
	else
	{
		return layer;
	}
}

void Entity::SetLayer(uint32_t layer)
{
	this->layer = layer;
}


/*******************************************************************************
*
*		Component System functions
*
*******************************************************************************/

void Entity::RemoveComponent(Component* comp)
{
	Log("About to remove comp '", comp, "'.");
	for(uint i = 0; i < components.size(); ++i)
	{
		if(components[i] == comp)
		{
			TriggerEvent("RemoveComponent", comp);
			TriggerEvent("OnSetComponent"+type::Descope(type::Name(comp)), comp);
			components.erase(components.begin()+i);
			break;
		}
	}
	Log("Removed component");
}

template<typename C>
void Entity::RemoveComponent()
{
	uint i = 0;
	for(auto& comp : components)
	{
		if(type::Name<C>() == type::Name(comp))
		{
			TriggerEvent("RemoveComponent", comp.get());
			comp->Destroy();
			components.erase(components.begin()+i);
			TriggerEvent("OnSetComponent"+type::Descope(type::Name<C>()), nullptr);
		}
		++i;
	}
}

void Entity::RemoveComponent(string type)
{
	for(uint i = 0; i < components.size(); ++i)
	{
		Component* comp = components[i];
		string t = type::Name(comp);
		if(t == type || type::Descope(t) == type)
		{
			TriggerEvent("RemoveComponent", comp);
			comp->Destroy();
			components.erase(components.begin()+i);
			TriggerEvent("OnSetComponent"+type::Descope(t), nullptr);
		}
	}
}

Entity* Entity::GetParent() const
{
	return parent;
}

Entity* Entity::GetParent(uint height) const
{
	Entity* result = GetParent();
	while(height > 1)
	{
		if(result)
		{
			result = result->GetParent();
			height--;
		}
		else
		{
			break;
		}
	}
	return result;
}

string Entity::GetName() const
{
	return name;
}

void Entity::Rename(string name)
{
	this->name = name;
	/// @todo do other stuff here?
//	TriggerEvent("Rename", name);
}

void Entity::Reparent(Entity* ent)
{
	GetParent()->RemoveChild(this, false);
	if(ent)
		ent->AddChild(this);
	else
		game->scene->GetRoot()->AddChild(this);
}

Entity* Entity::GetChild(uint idx)
{
	if(children.size() > idx)
	{
		return children[idx];
	}
	else
	{
		return nullptr;
	}
}

Entity* Entity::GetChild(string name, bool recursive)
{
	if(name.find("/") != -1)
	{
		// We're given a path to the entity
		std::deque<string> toks = Split(name, "", "/");
		Entity* result = nullptr;
		for(uint i = 0; i < toks.size(); ++i)
		{
			if(toks[i] == "/")
			{
				if(!result)
				{
					++i;
					result = GetChild(toks[i], false);
				}
				continue;
			}
			else if(toks[i] == "..")
			{
				if(result) result = result->GetParent();
			}
			else if(toks[i] == ".")
			{
				continue;
			}
			else
			{
				if(result)
				{
					result = result->GetChild(toks[i], false);
				}
			}
		}
		return result;
	}

	Entity* ent = nullptr;
	for(auto& child : children)
	{
		if(child->name == name)
		{
			ent = child;
		}
	}

	if(ent || !recursive)
	{
		return ent;
	}
	else
	{
		for(auto& child : children)
		{
			if((ent = child->GetChild(name, true)))
			{
				return ent;
			}
		}
	}
	return nullptr;
}

bool Entity::HasTag(string tag)
{
	for(auto& str : tags)
	{
		if(str == tag)
		{
			return true;
		}
	}
	return false;
}

/*******************************************************************************
*
*		Scripting
*
*******************************************************************************/


void Entity::Bind()
{/*
	Script::Class<Entity, Object> inst;
	inst.Var("name", &Entity::name);
//	inst.Var("id", &Entity::id);
//	inst.Var("layer", &Entity::layer);
	inst.Var("active", &Entity::active);
	inst.Var("lifetime", &Entity::lifetime);

	inst.Func("Init", &Entity::Init);
	inst.Func("Update", &Entity::Update);
	inst.Func("Destroy", &Entity::Destroy);
	inst.Func("AddComponent", (Component* (Entity::*)(string))&Entity::AddComponent);
	inst.Func("GetComponent", (Component* (Entity::*)(string))&Entity::GetComponent);
	inst.Func("RemoveComponent", (void (Entity::*)(string))&Entity::RemoveComponent);
	inst.Func("Clone", &Entity::Clone);
	inst.Func("GetChild", &Entity::GetChild);

	inst.Prop<string>("layer", &Entity::GetLayer, &Entity::SetLayer);
	inst.Prop<uint32_t>("id", &Entity::GetId);
	inst.Prop<Entity*>("parent", &Entity::GetParent, &Entity::SetParent);
*/}


/*******************************************************************************
*
*		Saving and Loading
*
*******************************************************************************/

void Entity::Save(Properties* prop) const
{
	Log("Saving entity to ", prop);
//	prop->type = "entity";
//	prop->id = name;
	prop->Set("name", name);

	prop->Set("id", id);
	prop->Set("tags", tags);
	prop->Set("layer", layer);
	prop->Set("active", active);
	prop->Set("lifetime", lifetime);

	for(auto& comp : components)
	{
		auto cp = prop->AddChild("");
		Log("Saving comp: ", type::Name(comp), " to ", cp);
		comp->Save(cp);
	}

	for(auto& child : children)
	{
		child->Save(prop->AddChild(""));
	}
}

void Entity::Load(Properties* prop)
{
	prop->Get("name", name);
	prop->Get("id", id);
//	prop->Get("tags", tags);
	prop->Get("layer", layer);
	prop->Get("active", active);
	prop->Get("lifetime", lifetime);

	Log("Entity::Load: Loaded basic properties.");

	string file = "";
	prop->Get("file", file);
	if(file != "")
	{
		Log("Loading entity from file path");
		ptr<Properties> fp {new Properties};
		fp->LoadFile(file);
		if(Properties* pc = fp->GetFirstChild())
		{
			Load(pc, true);
		}
	}

//	Properties* c = prop->GetChild("children");
	Entity* ent = nullptr;
	Component* component = nullptr;
	for(auto& child : prop->children)
	{
		if(child->type == "entity")
		{
			if((ent = GetChild(child->Get("name"))))
			{
				ent->Load(child);
			}
			else
			{
				AddChild()->Load(child);
			}
		}
		else if(child->type == "component")
		{
			if(child->name != "Transform")
			{
				Log("Entity::Load: Component type '"+child->name+"'");
				if((component = GetComponent(child->name)))
				{
					component->Load(child);
				}
				else
				{
					component = AddComponent(child->name);
					component->Load(child);
				}
				Log("Entity::Load: Added component, ", component);
			}
			else
			{
				if(transform)
				{
					component = transform;
					component->Load(child);
				}
				else
				{
					component = AddComponent("Transform");
					component->Load(child);
				}
			}
		}
	}

}

void Entity::Load(Properties* prop, bool init)
{
	Load(prop);
	if(init)
	{
		Init();
	}
}

Component *Entity::AddComponent(string type)
{
	Component* result = Component::NewFromString(type);
	if(result)
	{
		AddComponent(result);
	}
	return result;
}

}
