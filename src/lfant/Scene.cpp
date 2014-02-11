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
#include <lfant/Physics.h>
#include <lfant/Network.h>
#include <lfant/Renderer.h>

// External

namespace lfant
{

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	Subsystem::Init();
	if(game->defaultScene != "")
	{
		Log("About to load '"+game->defaultScene+"' as default.");
		LoadFile(game->defaultScene);
	}
}

void Scene::Update()
{
	Subsystem::Update();
	for(auto& ent : entities)
	{
		if(!ent || !ent->active)
			continue;

		ent->Update();
	}
}

void Scene::Render()
{
	Subsystem::Render();
	for(auto& ent : entities)
	{
		if(!ent || !ent->active)
			continue;

		ent->Render();
	}
}

void Scene::FixedUpdate()
{
	for(auto& ent : entities)
	{
		if(!ent || !ent->active)
			continue;

		ent->FixedUpdate();
	}
}

void Scene::Deinit()
{
	Subsystem::Deinit();
	Clear();
}

void Scene::Clear()
{
	for(auto& ent : entities)
	{
		Log("Scene::Destroy: Destroying ", ent->name);
		ent->Deinit();
	}
	entities.clear();
}

void Scene::RemoveEntity(Entity* ent, bool destroy)
{
	for(auto i = entities.begin(); i != entities.end(); ++i)
	{
		if(*i == ent)
		{
			Log("Scene::RemoveEntity: Touch.");
			if(!destroy)
			{
				*i = nullptr;
			}
			entities.erase(i);
			return;
		}
	}
	Log("Scene::RemoveEntity: Finished.");
}

Entity* Scene::GetEntity(string name, bool recursive) const
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

Entity* Scene::GetEntityById(uint32_t id, bool recursive) const
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
	return nullptr;
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

void Scene::Save(Properties* prop) const
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
	for(auto& i : prop->GetChildren("system"))
	{
		if(i->id == "Physics")
		{
			Log("Loading physics from scene");
			game->physics->Load(i);
		}
		else if(i->id == "Network")
		{
			game->network->Load(i);
		}
	/*	else if(i->id == "Renderer")
		{
			game->renderer->Load(i);
		}*/
	}

	Entity* ent = nullptr;
	for(auto& i : prop->GetChildren("entity"))
	{
		Log("About to spawn");
		ent = Spawn(nullptr, i->id);
		Log("Spawned the entity!");
		ent->Load(i);
	}
	Log("Loading scene took ", game->time->GetTime() - t, " seconds");
}

void Scene::LoadFile(string path)
{
	Subsystem::LoadFile(path);
	currentFile = path;
}

void Scene::AddEntity(Entity* ent)
{
	if(!ent->parent)
	{
		entities.push_back(ent);
		Log("Scene::AddEntity: pushed in.");
	}
	else
	{
		ent->parent->AddChild(ent);
	}
	ent->Init();
	TriggerEvent("AddEntity", ent);
	Log("Scene::AddEntity: Finished.");
}

Entity* Scene::Spawn(Entity* parent, string name)
{
	Entity* ent = new Entity;
	Log("Scene::Spawn: Allocated Entity pointer.");
	ent->parent = parent;
	Log("Scene::Spawn: Set parent.");
	ent->name = name;
	Log("Scene::Spawn: Set name.");
	AddEntity(ent);
	Log("Scene::Spawn: Finished.");
	return ent;
}

Entity* Scene::SpawnAndLoad(Properties* prop, Entity* parent, string name)
{
	Entity* ent = new Entity;
	ent->parent = parent;
	ent->name = name;
	ent->Load(prop);
	AddEntity(ent);

	return ent;
}

deque<Entity*> Scene::GetEntities(string tag) const
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

const deque<ptr<Entity>>& Scene::GetEntities() const
{
	return entities;
}

}
