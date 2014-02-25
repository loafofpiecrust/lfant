/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

#include <lfant/stdafx.h>

// External
//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/uuid_generators.hpp>
#include <forward_list>

// Internal
#include <lfant/Object.h>
//#include <lfant/Component.h>
#include <lfant/TypeInfo.h>
#include <lfant/Game.h>
#include <lfant/ptr.h>
#include <lfant/Properties.h>
//#include <lfant/Component.h>

namespace lfant {

namespace editor {
namespace gui
{
	class Inspector;
	class EntityTree;
}
}

class Component;
class Item;
class Transform;
class Entity;

bool EntityActive(Entity* ent);

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
class Entity final : public Object
{
	friend class lfant::editor::gui::Inspector;
	friend class lfant::editor::gui::EntityTree;

	friend class Scene;
	friend class Component;
	friend class Renderer;

public:
	Entity(const Entity& ent);
	virtual ~Entity();

	Entity& operator=(const Entity& other);

	virtual void Destroy();

	virtual void Save(Properties* prop) const;
	virtual void Load(Properties* prop);

	void Load(Properties* prop, bool init);

	static void ScriptBind();

	Entity* AddChild(string name = "");
	void RemoveChild(Entity* ent);

	/**
	 *	Adds a new component by instancing the given type.
	 *	@tparam C The class of component to add.
	 */
	template<typename C>
	C* AddComponent()
	{
		C* comp = GetComponent<C>();
		if(comp)
		{
			return comp;
		}
		else
		{
			comp = new C();
			AddComponent(comp);
			TriggerEvent("OnSetComponent"+type::Descope(type::Name(comp)), comp);
			return comp;
		}
	}

	Component* AddComponent(string type);

	/**
	 *	Removes a component from this Entity
	 *	@tparam C The type of component to remove.
	 */
	template<typename C>
	void RemoveComponent();
	void RemoveComponent(string type);

	/**
	 *	Clones an identical instance of this Entity.
	 */
	Entity* Clone(string name = "", Entity* parent = nullptr) const;
	void Clone(Entity* ent, string name = "", Entity* parent = nullptr) const;

	template<typename C>
	C* GetComponent()
	{
	//	string type = type::Name<C>();
		C* result = nullptr;
		for(ptr<Component>& comp : components)
		{
			if(!comp.get())
			{
				std::cout << "Wartf comp doesnt exist\n";
			}
			
			std::cout << "(" << this << ")::GetComponent() curr = " << type::Name(comp.get()) << "\n";
			
			result = dynamic_cast<C*>(comp.get());
			if(result)
			{
				std::cout << "Successful GetComponent() " << result << "\n";
				return result;
			}
		}
		return nullptr;
	}

	Component* GetComponent(string name) const;

	template<typename C>
	std::deque<C*> GetComponents() const
	{
		std::deque<C*> comps;
		for(auto& comp : components)
		{
			if(type::Name(comp) == type::Name<C>())
			{
				comps.push_back(dynamic_cast<C*>(comp.get()));
			}
		}
		return comps;
	}

	Entity* GetChild(string name, bool recursive = false) const;

	template<typename C>
	std::deque<C*> GetChildrenWithComponent(bool recursive = false) const
	{
		C* temp = nullptr;
		std::deque<C*> result;
		for(auto& child : children)
		{
			temp = child->GetComponent<C>();
			if(temp)
			{
				result.push_back(temp);
			}
		}
		return result;
	}
	
	const std::deque<ptr<Entity>>& GetChildren() const;

//	Entity* SpawnChild();

	bool HasTag(string tag) const;

	uint32_t GetId() const { return id; }

	uint32_t GetLayer() const;
	void SetLayer(uint32_t layer);

	Entity* GetParent() const;
	Entity* GetParent(uint height) const;
	void Reparent(Entity* ent);

	string GetName() const;
	void Rename(string name);

	Transform* transform;

	bool active = true;
	float lifetime = 0.0f;
	std::deque<string> tags;
	
	std::deque<ptr<Entity>> children;

private:
	Entity();

	virtual void Init();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void Deinit();

	/// @todo Make public?
	void AddChild(Entity* ent);
	void RemoveChild(Entity* ent, bool destroy);

	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);

	template<typename... P>
	void TriggerEventWithChildren(string name, P... args)
	{
		TriggerEvent(name, args...);
		for(auto& c : children)
		{
			if(!EntityActive(c)) continue;
			c->TriggerEventWithChildren(name, args...);
		}
	}

	template<typename... P>
	void TriggerEventWithParent(string name, P... args)
	{
		TriggerEvent(name, args...);
		if(parent && EntityActive(parent))
		{
			parent->TriggerEvent(name, args...);
		}
	}


	std::deque<ptr<Component>> components;

	string name = "root";

	/// Scene-unique identifier.
	uint32_t id = 0;

	/// The layer of this entity for primarily display filtering
	uint32_t layer = 0;

	Entity* parent = nullptr;

	bool useLifeTime = false;
};

/** @} */
/** @} */
}
