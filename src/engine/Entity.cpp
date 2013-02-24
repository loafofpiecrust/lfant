/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include "Entity.hpp"

// External
#include <stdarg.h>

// Internal
#include "Time.hpp"
#include "Engine.hpp"
#include "Scene.hpp"
#include "TypeInfo.hpp"
#include "Console.hpp"
#include "String.hpp"
#include "Component.hpp"

namespace sfs
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

	transform->SetDirection();

	for(auto& compo : components)
	{
		if(compo->IsEnabled())
		{
			compo->Update();
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
	children.push_back(unique_ptr<Entity>(ent));
}

void Entity::Destroy()
{
	for(auto compo : components)
	{
		compo->Destroy();
	}
	Log("Entity::Destroy: Components destroyed");
	OnDestroy();
	game->scene->RemoveEntity(name);
	delete this;
}

Entity* Entity::Clone(string name, Entity* parent, vec3 pos, vec3 rot)
{
	Entity* ent = game->scene->Spawn(name, parent, pos, rot);
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
	for(uint i = 0; i < components.size(); ++i)
	{
		if(CheckType(components[i], comp))
		{
			components.erase(components.begin() + i);
			comp->Destroy();
		}
	}
}

template<class T>
void Entity::RemoveComponent()
{
	for(uint i = 0; i < components.size(); ++i)
	{
		if(CheckType<T*>(components[i]))
		{
			components.erase(components.begin() + i);
			for(uint k = 0; k < components.size(); ++k)
			{
				components[k]->OnRemoveComponent(components[i]);
			}
			components[i]->Destroy();
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
			ent = child.get();
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

void Entity::SetActiveRecursive(bool active)
{
	this->active = active;
	for(uint i = 0; i < children.size(); ++i)
	{
		children[i]->active = active;
	}
}

}
