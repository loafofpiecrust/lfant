/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-07-17 by Taylor Snead
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


#include <lfant/Entity.h>

// External
#include <stdarg.h>

// Internal
#include <lfant/Time.h>

#include <lfant/Game.h>
#include <lfant/Scene.h>
#include <lfant/TypeInfo.h>

#include <lfant/Console.h>
#include <lfant/String.h>
#include <lfant/Transform.h>

#include <lfant/Component.h>

namespace lfant
{

Entity::Entity()
{
}

Entity::Entity(Entity* parent) :
	parent(parent)
{

}

Entity::~Entity()
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

	Log("Entity::Init: Touch.");

	transform = AddComponent<Transform>();

	if(lifeTime <= 0.0f)
	{
		useLifeTime = false;
	}
}

void Entity::Update()
{
	if(lifeTime > 0.0f)
	{
		useLifeTime = true;
	}
	if(useLifeTime == true)
	{
		lifeTime -= game->time->deltaTime;
		if(lifeTime <= 0.0f)
		{
			Destroy();
		}
	}

	GetComponent<Transform>()->SetDirection();

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

void Entity::AddChild(Entity* ent)
{
	children.push_front(ptr<Entity>(ent));
}

void Entity::RemoveChild(Entity *ent)
{
	children.remove(ptr<Entity>(ent));
}

void Entity::Destroy()
{
	Object::Destroy();

	for(auto& compo : components)
	{
		compo->Destroy();
	}
	Log("Entity::Destroy: Components destroyed");

	if(!parent)
	{
		game->scene->RemoveEntity(this);
	}
	else
	{
		parent->RemoveChild(this);
	}
}

Entity* Entity::Clone(string name, Entity* parent)
{
	Entity* ent = game->scene->Spawn(name, parent);
	//ent->components = components;
	/// @todo clone children here
	//ent->Init();
	return ent;
}

Component* Entity::GetComponent(string type)
{
	vector<string> spl = Split(type, ".: ", "");
	for(auto& comp : components)
	{
		if(Type(comp) == type || Type(comp) == spl[spl.size()-1])
		{
			return comp;
		}
	}
	return nullptr;
}

/*******************************************************************************
 *
 *		Component System functions
 *
 *******************************************************************************/

void Entity::RemoveComponent(Component* comp)
{
	components.remove(ptr<Component>(comp));
}

template<class T>
void Entity::RemoveComponent()
{
	for(auto& comp : components)
	{
		if(CheckType<T>(comp))
		{
			for(auto& comp2 : components)
			{
				comp2->OnRemoveComponent(comp);
			}
			comp->Destroy();
			components.remove(comp);
		}
	}
}

Entity* Entity::GetChild(string name, bool recursive)
{
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

/*******************************************************************************
 *
 *		Scripting
 *
 *******************************************************************************/

void Entity::Bind()
{
	/*
	SClass<Entity> ent;
	ent.Var("name", &Entity::name);
	ent.Var("tag", &Entity::tag);
	ent.Var("layer", &Entity::layer);
	ent.Var("lifeTime", &Entity::lifeTime);
	ent.Var("active", &Entity::active);

	ent.Prop("parent", &Entity::GetParent, &Entity::SetParent);

	ent.Func( "GetChild", &Entity::GetChild );
	ent.Func( "GetComponent", &Entity::GetComponent<Component> );
	*/
}

}
