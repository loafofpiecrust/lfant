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
#pragma once

#include <lfant/stdafx.h>

// External
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <forward_list>

// Internal
#include <lfant/Object.h>
//#include <lfant/Component.h>
#include <lfant/TypeInfo.h>
#include <lfant/Game.h>
#include <lfant/ptr.h>
#include <lfant/Properties.h>

namespace lfant
{
class Component;
class Item;
class Transform;

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *	This class is the basis for all Entities.
 *		Handles basic object and entity duties, like initialization,
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

	void Destroy();

	void Bind();
	void Save(Properties* prop);
	void Load(Properties* prop);

	/**
	 *	Adds a new component by instancing the given type.
	 *	@tparam C The class of component to add.
	 */
	template<typename C>
	C* AddComponent(Properties* prop = nullptr)
	{
		C* comp = new C();
		AddComponent(comp, prop);
		return comp;
	}

	Component* AddComponent(string type, Properties* prop = nullptr);

	void AddComponent(Component* comp, Properties* prop = nullptr);

	/**
	 *	Removes a component from this Entity
	 *	@tparam C The type of component to remove.
	 */
	template<typename C>
	void RemoveComponent();
	void RemoveComponent(Component* comp);

	/**
	 *	Clones an identical instance of this Entity.
	 */
	Entity* Clone(string name, Entity* parent = nullptr);

	template<typename C>
	C* GetComponent()
	{
		for(auto& comp : components)
		{
			if(Type(comp) == Type<C>())
			{
				return dynamic_cast<C*>(comp.get());
			}
		}
		return nullptr;
	}

	Component* GetComponent(string type);

	template<typename C>
	deque<C*> GetComponents()
	{
		deque<C*> comps;
		for(auto& comp : components)
		{
			if(Type(comp) == Type<C>())
			{
				comps += dynamic_cast<C*>(comp.get());
			}
		}
		return comps;
	}

	Entity* GetChild(string name, bool recursive = false);

	bool HasTag(string tag);

	Transform* transform = nullptr;

	/// Whether to update this Entity or not.
	bool active = true;

	/// 9-digit scene-unique identifier.
	const boost::uuids::uuid id = boost::uuids::random_generator() ();

	string name = "Entity";

	/// The identifying tags used for grouping.
	deque<string> tags;

	/// The layer of this entity for primarily display filtering
	string layer = "Default";

	float lifeTime = 0.0f;

	Entity* parent;

protected:

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy()
	{
	}

	void AddChild(Entity* ent);
	void RemoveChild(Entity* ent);
	void UnsafeDestroy();

private:
	Entity();
	virtual ~Entity();

	deque< ptr<Entity, Object::Delete> > children;
	deque< ptr<Component, Object::Delete> > components;
	bool useLifeTime = false;
};

/** @} */
/** @} */
}
