/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-20 by Taylor Snead
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


#include <lfant/Component.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/Entity.h>

namespace lfant {

map< string, Component* (Entity::*)(Properties*)> Component::componentRegistry __attribute__((init_priority(101)));

Component::Component()
{
}

Component::~Component()
{
}

void Component::Load(Properties* prop)
{
	prop->Get("enabled", enabled);
}

void Component::Save(Properties *prop)
{
	prop->type = "component";
	prop->id = RemoveScoping(Type(this));
	prop->Set("enabled", enabled);
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

void Component::Destroy()
{
	Object::Destroy();
	owner->RemoveComponent(this);
}

void Component::OnDestroy()
{
	Object::OnDestroy();
}

void Component::OnEnable()
{
}

void Component::OnDisable()
{
}

void Component::TriggerEvent(string name)
{
	owner->TriggerEvent(name);
	Object::TriggerEvent(name);
}

/*******************************************************************************
*		General functions
*		\area General
*******************************************************************************/

void Component::Enable(bool enable)
{
	enabled = enable;
	if(enabled)
	{
		OnEnable();
	}
	else
	{
		OnDisable();
	}
}

void Component::RegisterType(string name, Component* (Entity::*func)(Properties*))
{
//	Log("Registering component type '"+name+"', func: '", func, "'.");
	printf("Component::RegisterType: Touch.\n");
	if(func)
	{
		componentRegistry[name] = nullptr;
		cout << "Registering type with function ptr " << func << " to this current value: " << componentRegistry[name] << "\n";
		componentRegistry[name] = func;
	}
	else
	{
		printf("Component::RegisterType: Function is null.\n");
	}
}


bool Component::IsEnabled()
{
	return enabled;
}

}
