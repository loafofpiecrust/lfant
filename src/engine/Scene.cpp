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
		ent->Destroy();
	}
	entities.clear();
}

void Scene::RemoveEntity(string name)
{
	for(uint i = 0; i < entities.size(); ++i)
	{
		if(entities[i]->name == name)
		{
			entities.erase(entities.begin()+i);
		}
	}
}

/*Entity* Scene::GetEntity( uint32_t idx )
 {
 return entities[idx];
 }*/

Entity* Scene::GetEntity(string name, bool recursive)
{
	for(auto& ent : entities)
	{
		if(ent->name == name)
		{
			return ent.get();
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
	if(file == "")
	{
		file = name;
	}
	file.append(".scene");
	ofstream io;
	io.open(file);
	io << "<Scene name=\"" << "SceneName" << "\">\n";
	for(auto& ent : entities)
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
	Entity* ent {new Entity};
	ent->parent = parent;
	ent->transform->owner = ent;
	if(parent)
	{
		ent->transform->parent = parent->transform;
	}
	ent->transform->SetPosition(pos);
	ent->transform->SetRotation(rot);
	ent->name = name;
	ent->active = true;
	if(!parent)
	{
		Log("Scene::Spawn: Vector capacity: ", entities.capacity());
		if(entities.size() >= entities.capacity()-1)
		{
			entities.reserve(entities.size()+5);
			Log("Scene::Spawn: Had to resrve for Vector capacity: ", entities.capacity());
		}
		entities.push_back(unique_ptr<Entity>(ent));
	}
	else
	{
		parent->AddChild(ent);

	}
	ent->Init();
	return ent;
}

}
