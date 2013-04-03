/******************************************************************************
*
* LFANT Source
* Copyright (C) 2012-2013 by LazyFox Studios
* Created: 2012-09-02 by Taylor Snead
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/
#include <lfant/Scene.h>

// External

// Internal
#include <lfant/Entity.h>
#include <lfant/Console.h>
#include <lfant/Camera.h>
#include <lfant/Properties.h>

namespace lfant
{

Scene::Scene()
{
	// TODO Auto-generated constructor stub
}

Scene::~Scene()
{
	// TODO Auto-generated destructor stub
}

void Scene::Init()
{
}

void Scene::Update()
{
	for(auto& ent : entities)
	{
		if(ent->active)
		{
			ent->Update();
		}
	}
}

void Scene::OnDestroy()
{
	for(auto& ent : entities)
	{
		Log("Scene::Destroy: Destroying ", ent->name);
		ent->UnsafeDestroy();
	}
//	entities.clear();
}

void Scene::RemoveEntity(Entity* ent)
{
	for(uint i = 0; i < entities.size(); ++i)
	{
		if(entities[i] == ent)
		{
			Log("Scene::RemoveEntity: Touch.");
			entities.erase(entities.begin()+i);
			return;
		}
	}
	Log("Scene::RemoveEntity: Finished.");
}

Entity* Scene::GetEntity(string name, bool recursive)
{
	for(auto& ent : entities)
	{
		if(ent->name == name)
		{
			return ent;
		}
		if(recursive)
		{
			if(Entity* child = ent->GetChild(name, true))
			{
				return child;
			}
		}
	}
	return nullptr;
}

void Scene::Save(Properties* prop)
{
	prop->type = "scene";
	prop->id = name;

	for(auto& ent : entities)
	{
		ent->Save(prop->AddChild());
	}
}

void Scene::Load(Properties *prop)
{
	Log("Scene::Load: Loaded scene node");
	deque<Properties*> ents = prop->GetChildren("entity");
	Log("Scene::Load: Filled entity list");
	Entity* ent = nullptr;
	for(auto& i : ents)
	{
		ent = Spawn(i->id);
		Log("Spawned the entity!");
		ent->Load(i);
	}
}

Entity* Scene::Spawn(string name, Entity* parent)
{
	Entity* ent = new Entity;
	Log("Scene::Spawn: Allocated Entity pointer.");
	ent->parent = parent;
	Log("Scene::Spawn: Set parent.");
	ent->name = name;
	Log("Scene::Spawn: Set name.");
	ent->active = true;
	Log("Scene::Spawn: Set active.");
	Log("Scene::Spawn: About to Init, ", ent->name);

	ent->Init();
	Log("Scene::Spawn: Initialised, ", ent);
	if(!parent)
	{
		entities.push_front(ent);
		Log("Scene::Spawn: pushed in.");
	}
	else
	{
		parent->AddChild(ent);
	}
	return ent;
}

deque<Entity*> Scene::GetEntities(string tag)
{
	deque<Entity*> result;
	for(auto& ent : entities)
	{
		if(ent->HasTag(tag))
		{
			result.push_back(ent);
		}
	}
	return result;
}

}
