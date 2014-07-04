/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include "./Entity.h"

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
#include "ScriptComp.h"
#include "FileSystem.h"

// External
#include <stdarg.h>

namespace lfant {

bool EntityActive(Entity* ent)
{
	return ent->enabled;
}

Entity::Entity(Scene* scene) :
	scene(scene)
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
	Entity* ent = new Entity(scene);
	Clone(ent, name, parent);
	return ent;
}

void Entity::Clone(Entity* ent, string name, Entity* parent) const
{
	if(name == "") name = this->name;
	if(!parent) parent = this->parent;

	ent->name = name;
	ent->parent = parent;
	scene->GetRoot()->AddChild(ent);

	for(auto& c : components)
	{
		c->Clone(ent);
	}
	for(auto& c : children)
	{
		c->Clone(c->name, ent);
	}
}

/*******************************************************************************
*
*		Generic Entity functions
*
*******************************************************************************/

void Entity::Init()
{
	Object::Init();

//	if(!transform)
//	{
	//	transform = AddComponent(new Transform);
//	}

	if(id == 0)
	{
		// Generate new id?
		id = scene->GenerateEntityId();
	}

	std::cout << "\n\n";
	for(auto& comp : components)
	{
		std::cout << "comp " << comp << "\n";
		if(comp)
		{
			std::cout << type::Name(comp) << "\n";
		}
	}
	std::cout << "\n\n";
}

void Entity::Update()
{
	if(lifetime > 0.0f)
	{
		lifetime -= GetGame()->time->deltaTime;
		if(lifetime <= 0.0f)
		{
			wantsToDie = true;
		}
	}

	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->Update();
		}
	}

	for(auto& child : children)
	{
		if(child->IsEnabled())
		{
			child->Update();
		}
	}
}

void Entity::Render()
{
	for(auto& comp : components)
	{
		if(comp->IsRendered() && comp->IsEnabled())
		{
			comp->Render();
		}
	}

	for(auto& child : children)
	{
		if(child->IsEnabled())
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
		if(child->IsEnabled())
		{
			child->FixedUpdate();
		}
	}
}

void Entity::AddChild(Entity* ent)
{
	children.push_back(ent);
	ent->parent = this;
	std::cout << name << " adding child " << ent << " to " << scene << "\n";
	ent->scene = this->scene;
}

Entity* Entity::AddChild(string name)
{
	Entity* ent = new Entity(scene);
	ent->name = name;
	AddChild(ent);
	scene->TriggerEvent("AddEntity", ent);
	return ent;
}

void Entity::RemoveChild(Entity* ent)
{
	RemoveChild(ent, true);
}

void Entity::RemoveChild(Entity* ent, bool destroy)
{
	//	children.remove(ptr<Entity>(ent));
	for(auto i = children.begin(); i != children.end(); ++i)
	{
		if(*i == ent)
		{
			if(!destroy) (*i).release();
			children.erase(i);
			break;
		}
	}

}

void Entity::AddComponent(Component* comp)
{
	if(!comp) return;

	// @note Needed to call comp->Deinit() here?
	if(comp->GetOwner())
	{
		if(comp->GetOwner() == this)
		{
			comp->Init();
			return;
		}

		for(auto it = comp->GetOwner()->components.begin(); it != comp->GetOwner()->components.end(); ++it)
		{
			if(*it == comp)
			{
				*it = nullptr;
				comp->GetOwner()->components.erase(it);
				break;
			}
		}
	}

	GetGame()->Log("Entity::AddComponent(", comp, ") to ", this);
	components.push_back(comp);
	comp->owner = this;
	comp->Init();
	TriggerEvent("OnSetComponent"+type::Descope(type::Name(comp)), comp);
	TriggerEvent("AddComponent", comp);
}

void Entity::Deinit()
{
	Object::Deinit();

	GetGame()->Log("Deiniting entity '"+GetName()+"'.");

	for(auto& child : children)
	{
		child->Deinit();
	}
	children.clear();

	for(auto it = components.begin(); it != components.end(); ++it)
	{
		auto& compo = *it;
		if(compo) compo->Deinit();
		if(type::Name(compo.get()) == type::Name<ScriptComp>())
		{
		//	((ScriptComp*)compo.get())->DestroyFromScript();
			ScriptComp::DestroyFromScript((ScriptComp*)compo.get());
			(*it).release();
		//	components.erase(it);
		}
	}
	components.clear();
}

void Entity::DestroyDeadChildren()
{
	for(uint32 i = 0; i < children.size(); ++i)
	{
		Entity* child = children[i];
		if(child->wantsToDie)
		{
			child->Deinit();
			children.erase(children.begin()+i);
			--i;
		}
		else
		{
			child->DestroyDeadChildren();
		}
	}
}

void Entity::Destroy()
{
	Object::Destroy();
	if(parent) parent->RemoveChild(this);
}

Component* Entity::GetComponent(string name)
{
	auto reg = Component::typeRegistry.Get(name);
//	std::cout << "getting from typeRegistry took " << hclock::now()-start << "\n";
	if(!reg)
	{
		GetGame()->Log("Component type not registered");
		return nullptr;
	}

	string ctype = "";
	for(uint32 i = 0; i < components.size(); ++i)
	{
		Component* comp = components[i];
		if(!comp)
		{
			components.erase(components.begin()+i);
			--i;
			continue;
		}
		const std::type_info& ti = typeid(*comp);

	//	ctype = type::Name(comp);
	//	auto reg2 = Component::typeRegistry.Get(typeid(comp));
	//	if(!reg2) continue; // Very rare, but provide for it just in case

	//	start = hclock::now();
	//	std::cout << "checking comp '" << typeid(*comp).name() << "'\n";
		if(reg->typeInfo == ti || reg->Inherits(ti))
		{
			return comp;
		}
	//	std::cout << "two typeid comparisons took " << hclock::now()-start << "\n";
	}
	return nullptr;
}

uint32 Entity::GetLayer() const
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

void Entity::SetLayer(uint32 layer)
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
	for(auto i = components.begin(); i != components.end(); ++i)
	{
		if(*i == comp)
		{
			TriggerEvent("RemoveComponent", comp);
			TriggerEvent("OnSetComponent"+type::Descope(type::Name(comp)), comp);
			components.erase(i);
			GetGame()->Log("Removed component ", comp);
			break;
		}
	}
}

template<typename C>
void Entity::RemoveComponent()
{
	for(auto i = components.begin(); i != components.end(); ++i)
	{
		Component* comp = (*i).get();
		if(typeid(C) == typeid(*comp))
		{
			TriggerEvent("RemoveComponent", comp);
			comp->Destroy();
			components.erase(i);
			TriggerEvent("OnSetComponent"+type::Descope(type::Name<C>()), nullptr);
		}
	}
}

void Entity::RemoveComponent(string type)
{
	auto reg = Component::typeRegistry.Get(type);
	if(!reg) return;

	for(auto i = components.begin(); i != components.end(); ++i)
	{
		Component* comp = i->get();
		if(reg->Is(typeid(*comp)))
		{
			TriggerEvent("RemoveComponent", comp);
			comp->Destroy();
			components.erase(i);
			TriggerEvent("OnSetComponent"+type, nullptr);
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

void Entity::Enable(bool on)
{
	enabled = on;
}

bool Entity::IsEnabled() const
{
	return enabled;
}

Game* Entity::GetGame() const
{
//	std::cout << name << " checking " << scene << "\n";
	return scene->GetGame();
}

void Entity::Reparent(Entity* ent)
{
//	GetGame()->Log("Reparenting from ", GetParent());
	GetParent()->RemoveChild(this, false);
	if(ent)
		ent->AddChild(this);
	else
		scene->GetRoot()->AddChild(this);
}

Entity* Entity::GetChild(string name, bool recursive) const
{
	if(name.find("/") != -1)
	{
		// We're given a path to the entity
		std::deque<string> toks = Split(name, "/", "");
		Entity* result = nullptr;
		if(name[0] == '/')
		{
			result = GetGame()->scene->GetRoot();
		}
		for(uint i = 0; i < toks.size(); ++i)
		{
			if(toks[i] == ".")
			{
				continue;
			}
			else
			{
				if(toks[i] == "..")
				{
					if(result) result = result->GetParent();
				}
				else
				{
					result = result->GetChild(toks[i], false);
				}

				if(!result)
				{
					GetGame()->Log("Entity '"+name+"' not found in "+GetName());
					return nullptr;
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
			ent = child->GetChild(name, true);
			if(ent)
			{
				return ent;
			}
		}
	}
	return nullptr;
}

const std::deque<ptr<Entity>>& Entity::GetChildren() const
{
	return children;
}

bool Entity::HasTag(string tag) const
{
	for(auto& str : tags)
	{
		if(str == tag) return true;
	}
	return false;
}

/*******************************************************************************
*
*		Scripting
*
*******************************************************************************/


void Entity::ScriptBind()
{
	Script::Class<Entity, Object> inst;
//	inst.Var("layer", &Entity::layer);
	inst.Var("active", &Entity::enabled);
	inst.Var("lifetime", &Entity::lifetime);
	inst.Var("transform", &Entity::transform);

	inst.Func("Init", &Entity::Init);
	inst.Func("Update", &Entity::Update);
	inst.Func("Destroy", &Entity::Destroy);
	inst.Func("AddComponent", (Component*(Entity::*)(string))&Entity::AddComponent);
	inst.Func("GetComponent", (Component*(Entity::*)(string))&Entity::GetComponent);
	inst.Func("RemoveComponent", (void (Entity::*)(string))&Entity::RemoveComponent);
//	inst.Func("Clone", &Entity::Clone);
	inst.Func("GetChild", &Entity::GetChild);

	inst.Prop("name", &Entity::GetName, &Entity::Rename);
	inst.Prop<uint32>("layer", &Entity::GetLayer, &Entity::SetLayer);
	inst.Prop<uint32>("id", &Entity::GetId);
	inst.Prop<Entity*>("parent", &Entity::GetParent, &Entity::Reparent);

	inst.Bind();
}


/*******************************************************************************
*
*		Saving and Loading
*
*******************************************************************************/

void Entity::Serialize(Properties* prop)
{
//	prop->Value("name", name);
	if(!prop->name.empty()) name = prop->name;
	prop->Value("id", &id);
	prop->Value("tags", &tags);
	prop->Value("layer", &layer);
	prop->Value("active", &enabled);
	prop->Value("lifetime", &lifetime);

	if(prop->mode == Properties::Mode::Input)
	{
		std::deque<string> files;
		prop->Value("files", &files);
		for(auto& file : files)
		{
			Properties fp;
			fp.LoadFile(file);
			if(Properties* pc = fp.GetChild(0))
			{
				pc->SetMode(Properties::Mode::Input);
				Serialize(pc);
			}
		}
	}

//	prop->ValueArray<ptr<Entity>>("Entity", children, [&](ptr<Entity>& ent, Properties* prop)
	for(auto& child : prop->children)
	{
		if(!child->IsType("Entity")) continue;

		Entity* ent = nullptr;
		if(child->mode == Properties::Mode::Input)
		{
			 if((ent = GetChild(child->name)))
			 {
				 ent->Serialize(child);
			 }
			 else
			 {
				 ent = AddChild(child->name);
				 ent->Serialize(child);
			 }
		}
		else
		{
		//	ent->Serialize(prop->Child("Entity", ""));
		}
	}

//	prop->ValueArray<ptr<Component>>("Component", components, [&](ptr<Component>& component, Properties* child)
	if(prop->mode == Properties::Mode::Input)
	{
		for(auto& child : prop->children)
		{
			if(!child->IsType("Component")) continue;

			Component* component = nullptr;
			if(child->name == "Script")
			{
				string file = "";
				child->Value("file", file);
				component = ScriptComp::LoadScript(GetGame(), GetGame()->GetAssetPath(file).string());
				AddComponent(component);
			}
			else if(child->name == "Transform")
			{
				if(transform)
				{
					transform->Serialize(child);
				}
				else
				{
					transform = AddComponent<Transform>();
					transform->Serialize(child);
				}
			}
			else
			{
				component = GetComponent(child->name);
				if(component)
				{
					component->Serialize(child);
				}
				else
				{
					component = AddComponent(child->name);
					if(component) component->Serialize(child);
				}
				if(component) GetGame()->Log("Entity::Serialize: Added component, ", component, " to ", component->GetOwner());
			}
		}
	}
	else
	{
		for(auto& comp : components)
		{
			Properties* pc = prop->Child("Component", "");
			comp->Serialize(pc);
		}
	}

	if(prop->mode == Properties::Mode::Input)
		Init();
}

void Entity::Serialize(Properties* prop, bool init)
{
	Serialize(prop);
	if(init)
	{
		Init();
	}
}

Component *Entity::AddComponent(string type)
{
	std::cout << "trying to add comp "+type+"\n";
	Component* result = nullptr;
	result = Component::NewFromString(type);
	if(result)
	{
		AddComponent(result);
	}
	else
	{
		string type_script = type;
		replace_all(type_script, "::", "/");
		fs::path p = GetGame()->GetAssetPath("scripts/"+type_script+".nut");
		if(fs::exists(p))
		{
			result = ScriptComp::LoadScript(GetGame(), p.string());
			AddComponent(result);
		}
	}
	return result;
}

}
