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
#pragma once
#include "stdafx.hpp"

// External
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

// Internal
#include "Object.hpp"
#include "Transform.hpp"
#include "Component.hpp"
#include "TypeInfo.hpp"
#include "Engine.hpp"

namespace sfs
{
class Component;
class Item;

/** @addtogroup Engine
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *	This class is the basis for all Entities.
 *	@details
 *		Entity handles basic object and entity duties, like initialization,
 *		updating every frame, then cleaning up on destruction. Cloning is also
 *		supported by default. Entities can have functionality added to them
 *		by adding Components to their list of Component instances. Those
 *		are modules of functionality and data ready to be added onto an Entity.
 *		This prefers aggregation over inheritance.
 *	@todo
 *		Organize...
 */
class Entity : public Object
{
	friend class Scene;

public:
	Entity();
	Entity(Entity* parent);
	virtual ~Entity();

	virtual void Bind();
	virtual void Destroy();

	/**
	 *	Adds a new component by instancing the given type.
	 *	@tparam C The class of component to add.
	 */
	template<typename C>
	C* AddComponent()
	{
		C* comp = new C();
		comp->owner = this;
		comp->transform = transform;
		if(static_cast<Component*>(comp))
		{
			components.push_back(comp);
		}
		comp->Init();
		return comp;
	}

	/**
	 *	Removes a component from this Entity
	 *	@tparam T The type of component to remove.
	 */
	template<typename C>
	void RemoveComponent();
	void RemoveComponent(Component* comp);

	/**
	 *	Clones an identical instance of this Entity.
	 */
	Entity* Clone(string name, Entity* parent = nullptr, vec3 pos = vec3(0), vec3 rot = vec3(0));

	template<typename C>
	C* GetComponent()
	{
		for(auto & comp : components)
		{
			if(Type(comp) == Type<C>())
			{
				return dynamic_cast<C*>(comp);
			}
		}
		return nullptr;
	}

	Component* GetComponent(string type);

	template<typename C>
	vector<C*> GetComponents()
	{
		vector<C*> comps;
		for(auto & comp : components)
		{
			if(CheckType<C*>(comp))
			{
				comps += dynamic_cast<C*>(comp);
			}
		}
		return comps;
	}

	void AddChild(Entity* ent);
	Entity* GetChild(string name, bool recursive = false);

	void SetActiveRecursive(bool active);

	vector<Component*> components;

	/// Whether to update this Entity or not.
	bool active = true;

	Transform* transform = { new Transform };

	/// 9-digit scene-unique identifier.
	boost::uuids::uuid id = boost::uuids::random_generator()();

	string name = "Entity";

	/// The identifying tag. as a group.
	string tag = "Tag";

	/// The layer of this entity for primarily display filtering
	string layer = "Default";

	float lifeTime = 0.0f;

protected:

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy()
	{
	}

private:

	vector<Entity*> children;
	bool useLifeTime = true;

public:
	// Properties
	Entity* parent;
};

/** @} */
/** @} */
}
