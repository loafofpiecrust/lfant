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

// External
#include <stdarg.h>

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
	++childCount;
}

void Entity::RemoveChild(Entity* ent)
{
	children.remove(ptr<Entity>(ent));
	--childCount;
}

void Entity::AddComponent(Component* comp)
{
	components.push_front(ptr<Component>(comp));
	++componentCount;
}

void Entity::UnsafeDestroy()
{
	Object::Destroy();


	if(componentCount > 0)
	{
		Log("Entity::Destroy: Destroying ", componentCount,"components.");
		for(auto& compo : components)
		{
			compo->Destroy();
		}
	}

	if(childCount > 0)
	{
		Log("Entity::Destroy: Destroying ", childCount,"children.");
		for(auto& child : children)
		{
			child->UnsafeDestroy();
		}
	}
}

void Entity::Destroy()
{
	Log("Entity::Destroy: Touch.");
	Object::Destroy();
	Log("Entity::Destroy: super called.");

	if(componentCount > 0)
	{
		Log("Entity::Destroy: Destroying ", componentCount,"components.");
		for(auto& compo : components)
		{
			compo->Destroy();
		}
	}
	//	components.clear();
	Log("Entity::Destroy: Components destroyed.");

	if(childCount > 0)
	{
		Log("Entity::Destroy: Destroying ", childCount,"children.");
		for(auto& child : children)
		{
			child->Destroy();
		}
	}

	if(!parent)
	{
		Log("Entity::Destroy: Removing from scene.");
		game->scene->RemoveEntity(this);
	}
	else
	{
		Log("Entity::Destroy: Removing from parent.");
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
	Log("About to remove comp '", comp, "'.");
//	components.remove(ptr<Component>(comp));
	for(auto& cp : components)
	{
		if(cp == comp)
		{
			components.remove(cp);
			break;
		}
	}
	--componentCount;
	printf("Removed component\n");
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
			--componentCount;
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
	   Script::Class obj;
	   obj.Create<Entity>("Entity", true);

	   obj.Method("void Destroy()", &Entity::Destroy);
	 */
}


/*******************************************************************************
*
*		Saving and Loading
*
*******************************************************************************/

void Entity::Save(Properties* prop)
{
	prop->type = "entity";
	prop->id = name;

	prop->Set("tag", tag);
	prop->Set("layer", layer);
	prop->Set("active", active);
	prop->Set("lifeTime", lifeTime);

	for(auto& comp : components)
	{
		comp->Save(prop->AddChild());
	}

	for(auto& child : children)
	{
		child->Save(prop->AddChild());
	}
}

void Entity::Load(Properties* prop)
{
	tag =		prop->Get<string>("tag");
	layer =		prop->Get<string>("layer");
	active =	prop->Get<bool>("active");
	lifeTime =	prop->Get<float>("lifeTime");

	Log("Entity::Load: Loaded basic properties.");

	vector<Properties*> c;
	c = prop->GetChildren("component");
	Log("Entity::Load: Component nodes loaded.");
	Component* component = nullptr;
	for(auto& comp : c)
	{
		component = AddComponent(comp->id);
		component->Load(comp);
	}

	c = prop->GetChildren("entity");
	Log("Entity::Load: Children nodes loaded.");
	Entity* ent = nullptr;
	for(auto& child : c)
	{
		ent = game->scene->Spawn(child->id, this);
		ent->Load(child);
	}

	Log("Entity::Load: Finished.");

}

Component *Entity::AddComponent(string type)
{
	printf("Adding comp via string of type\n");
	auto val = Component::componentRegistry[type];
	Component* result = nullptr;
	if(val == nullptr)
	{
		result = nullptr;
	}
	else
	{
		result = (this->*val)();
	}

	if(type == "Camera")
	{
		game->scene->mainCamera = dynamic_cast<Camera*>(result);
	}

	return result;
}

}
