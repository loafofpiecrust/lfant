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

Scene::Scene(Game* game) :
	Subsystem(game)
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	Subsystem::Init();

	root = new Entity(this);
	if(game->defaultScene != "")
	{
		GetGame()->Log("About to load '"+game->defaultScene+"' as default.");
		LoadFile(game->defaultScene);
	}
}

void Scene::Update()
{
	Subsystem::Update();
	GetRoot()->Update();
	GetRoot()->DestroyDeadChildren();
}

void Scene::Render()
{
	Subsystem::Render();
	root->Render();
}

void Scene::FixedUpdate()
{
	root->FixedUpdate();
}

void Scene::Deinit()
{
	Subsystem::Deinit();
	root->Deinit();
	root.reset();
}

void Scene::Clear()
{
}

uint32 Scene::GenerateEntityId()
{
	return ++currentId;
}

void Scene::Serialize(Properties *prop)
{
	double t = game->time->GetTime();
	currentId = 0;
	GetGame()->Log("Scene::Serialize: Loading scene node");
	if(prop->GetMode() == Properties::Mode::Input)
	{
		for(uint32 i = 0; i < prop->GetChildCount(); ++i)
		{
			Properties* p = prop->GetChild(i);
			if(p->IsType("System"))
			{
				if(p->name == "Physics")
				{
					GetGame()->Log("Loading physics from scene");
					game->physics->Serialize(p);
				}
				else if(p->name == "Network")
				{
					game->network->Serialize(p);
				}
			/*	else if(i->id == "Renderer")
				{
					game->renderer->Load(i);
				}*/
			}
		}
	}

	GetRoot()->Serialize(prop);

	GetGame()->Log("Serializing scene took ", game->time->GetTime() - t, " seconds");
}

void Scene::LoadFile(string path)
{
	Subsystem::LoadFile(path);
	currentFile = path;
}

std::deque<Entity*> Scene::GetEntities(string tag) const
{
	std::deque<Entity*> result;
	for(auto& ent : root->children)
	{
		if(ent->HasTag(tag))
		{
			result.push_back(ent.get());
		}
	}
	return result;
}
}
