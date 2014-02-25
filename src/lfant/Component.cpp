/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-20 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/


#include <lfant/Component.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/Entity.h>
#include <lfant/ScriptSystem.h>

namespace lfant {

IMPLEMENT_REGISTRY(Component)
//TypeRegistry<Component> Component::registry __attribute__((init_priority(101)));
//qumap<string, Component* (*)()> Component::componentRegistry __attribute__((init_priority(101)));

Component::Component()
{
}

Component::Component(const Component& other)
{
	other.Clone(this, nullptr);
}

Component::~Component()
{
}
/*
Component& Component::operator=(const Component& other)
{
	other.Clone(this);
	return *this;
}
*/
void Component::Clone(Component* comp, Entity* owner) const
{
	Properties prop;
	Save(&prop);
	comp->Load(&prop);
	if(owner) owner->AddComponent(comp);
}

Component* Component::Clone(Entity* owner) const
{
	Component* comp = Component::NewFromString(type::Descope(type::Name(this), "lfant"));
	owner->AddComponent(comp);
}

void Component::Load(Properties* prop)
{
	prop->Get("enabled", enabled);
}

void Component::Save(Properties *prop) const
{
	/// @todo Implement Properties::SetName()
//	prop->SetName("component");
	prop->type = "component";
	prop->name = type::Descope(type::Name(this));
	prop->Set("enabled", enabled);
	
//	prop->LoadFile("suckit");
	
}

void Component::Init()
{
	Object::Init();
	Log("Component::Init: owner = ", owner);
}

void Component::Update()
{
	Object::Update();
}

void Component::PostUpdate()
{
}

void Component::Render()
{
}

void Component::Destroy()
{
	Object::Destroy();
	owner->RemoveComponent(this);
}

void Component::Deinit()
{
	Object::Deinit();
}
/*
void Component::OnEnable()
{
}

void Component::OnDisable()
{
}
*/
void Component::TriggerEvent(string name)
{
	owner->TriggerEvent(name);
	Object::TriggerEvent(name);
}

/*******************************************************************************
*		General functions
*		\area General
*******************************************************************************/

void Component::SetOwner(Entity* ent)
{
	ent->AddComponent(this);
}

Entity* Component::GetOwner() const
{
	return owner;
}

void Component::Enable(bool enable)
{
	enabled = enable;
}


bool Component::IsEnabled()
{
	return enabled;
}

void Component::ScriptBind()
{
	Script::Class<Component, Object> inst;

	inst.Prop("owner", &Component::GetOwner, &Component::SetOwner);

	inst.Func("Init", &Component::Init);
	inst.Func("Update", &Component::Update);
	inst.Func("Deinit", &Component::Deinit);
	inst.Func("Destroy", &Component::Destroy);
	inst.Func("IsEnabled", &Component::IsEnabled);
	inst.Func("Enable", &Component::Enable);
	
	inst.Bind("ComponentBase");
}

}
