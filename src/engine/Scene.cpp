/******************************************************************************
 *
 * ShadowFox Engine Source
 * Copyright (C) 2012-2013 by ShadowFox Studios
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
#include "Scene.hpp"

// External

// Internal
#include "Entity.hpp"
#include "Console.hpp"
#include "Camera.hpp"

namespace sfs
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
	for(auto ent : entities)
	{
		if(ent->active)
		{
			ent->Update();
		}
	}
}

void Scene::OnDestroy()
{
	for(uint i = 0; i < entities.size(); ++i)
	{
		Log("Scene::Destroy: Destroying ", entities[i]->name);
		entities[i]->Destroy();
	}
}

/*Entity* Scene::GetEntity( uint32_t idx )
 {
 return entities[idx];
 }*/

Entity* Scene::GetEntity(string name, bool recursive)
{
	for(Entity* ent : entities)
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

void Scene::Save(string file)
{
	ofstream io;
	io.open(file);
	io << "<Scene name=\"" << "SceneName" << "\">\n";
	for(Entity* ent : entities)
	{
		io << "\t<Entity name=\"" << ent->name << "\">\n";
		vector<pair<string, string>> vars ;//= ent->Serialize();
		for(auto& str : vars)
		{
			io << "\t\t<Var name=\"" << str.first << "\" value=\"" << str.second << "\"/>\n";
		}
		io << "\t</Entity>\n";
	}
	io << "</Scene>";
	io.close();
}

Entity *Scene::Spawn(string name, Entity *parent, vec3 pos, vec3 rot, vec3 scale)
{
	Entity* ent = new Entity;
	ent->parent = parent;
	ent->transform->owner = ent;
	if(parent != nullptr)
	{
		ent->transform->parent = parent->transform;
	}
	ent->transform->position = pos;
	ent->transform->rotation = rot;
	ent->name = name;
	if(parent == nullptr)
	{
		entities.push_back(ent);
	}
	ent->Init();
	return ent;
}

}
