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

#include <lfant/Entity.h>

namespace lfant
{

map< string, Component* (Entity::*)() > Component::componentRegistry;

Component::Component(Entity* owner) :
	owner(owner)
{
}

Component::~Component()
{
}

void Component::Load(Properties* props)
{
	enabled = props->Get<bool>("enabled");
}

void Component::Init()
{
	Object::Init();
}

void Component::Destroy()
{
	Object::Destroy();
	owner->RemoveComponent(this);
}

void Component::Trigger(string name)
{
	owner->Trigger(name);
	Object::Trigger(name);
}

/*******************************************************************************
*		General functions
*		\area General
*******************************************************************************/

void Component::SetEnabled(bool enable)
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


bool Component::IsEnabled()
{
	return enabled;
}

}
