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

// Internal
#include <lfant/Entity.h>
#include <lfant/Console.h>
#include <lfant/Camera.h>
#include <lfant/Properties.h>
#include <lfant/Time.h>

// External

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
		if(!ent)
		{
			continue;
		}
		
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

void Scene::RemoveEntity(Entity* ent, bool destroy)
{
	for(uint i = 0; i < entities.size(); ++i)
	{
		if(entities[i] == ent)
		{
			Log("Scene::RemoveEntity: Touch.");
			if(!destroy)
			{
				entities[i] = nullptr;
			}
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

Entity* Scene::GetEntityById(uint32_t id, bool recursive)
{
	for(auto& ent : entities)
	{
		if(ent->GetId() == id)
		{
			return ent;
		}
		if(recursive)
		{
			for(auto& child : ent->children)
			{
				if(child->GetId() == id)
				{
					return child;
				}
			}
		}
	}
}

uint32_t Scene::GenerateEntityId()
{
	/*
	uint32_t result = 1;
	for(auto& ent : entities)
	{
		if(ent->GetId() == result)
		{
			++result;
		}
		for(auto& child : ent->children)
		{
			if(child->GetId() == result)
			{
				++result;
			}
		}
	}
	return result;
	*/
	return ++currentId;
}

void Scene::Save(Properties* prop)
{
	double t = game->time->GetTime();
	prop->type = "scene";
	prop->id = name;

	for(auto& ent : entities)
	{
		ent->Save(prop->AddChild());
	}
	Log("Saving scene took ", game->time->GetTime() - t, " seconds");
}

void Scene::Load(Properties *prop)
{
	double t = game->time->GetTime();
	currentId = 0;
	Log("Scene::Load: Loading scene node");
	Entity* ent = nullptr;
	for(auto& i : prop->GetChildren("entity"))
	{
		ent = Spawn(i->id);
		Log("Spawned the entity!");
		ent->Load(i);
	}
	Log("Loading scene took ", game->time->GetTime() - t, " seconds");
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
		entities.push_back(ent);
		Log("Scene::Spawn: pushed in.");
	}
	else
	{
		parent->AddChild(ent);
	}
	return ent;
}

Entity* Scene::SpawnAndLoad(Properties* prop, string name, Entity* parent)
{
	Entity* ent = new Entity;
	ent->parent = parent;
	ent->name = name;
	ent->active = true;
	ent->Load(prop);
	ent->Init();
	
	if(!parent)
	{
		entities.push_back(ent);
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
