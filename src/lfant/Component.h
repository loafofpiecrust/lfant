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

// Internal
#include <lfant/Object.h>
#include <lfant/Entity.h>

namespace lfant {

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

//#define REGISTER_COMP(comp) static bool BOOST_PP_CAT( comp, __regged ) = componentRegistry.insert( BOOST_PP_STRINGIZE(comp), &Entity::AddComponent<comp>());
#define DECLARE_COMP(type) \
	friend class lfant::Entity;\
	static RegistryEntry<type> _registryEntry;

#define IMPLEMENT_COMP(type) \
	Component::RegistryEntry<type> type::_registryEntry {#type};


/**	The base class for all Entity Components.
 *		Component is the basis for all things to be attached to
 *		Entity instances. These Components can be added or removed
 *		at will to modify the overall capable functionality of the
 *		owner Entity. This supports an aggregate over inheritance
 *		engine structure.
 *	@todo
 *		Make a name generator.
 *		Organize!
 */
class Component : public Object
{
	friend class Entity;
protected:

	template<typename T>
	class RegistryEntry
	{
	public:
		RegistryEntry(string type)
		{
			Component::componentRegistry[type] = (Component* (Entity::*)(Properties*))&Entity::AddComponent<T>;
		}
	};

public:
	virtual ~Component();

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop);

	/**
	 *	Returns whether this component is enabled.
	 */
	bool IsEnabled();

	/**
	 *	Enables or disables this component.
	 */
	void Enable(bool enable = true);


	/// The owner of this Component.
	Entity* owner = nullptr;

protected:
	Component();

	static map<string, Component* (Entity::*)(Properties*) > componentRegistry __attribute__((init_priority(101)));

	/**
	 *	Registers a component type by string, only used by the IMPLEMENT_COMP macro.
	 *	@param name The typename
	 */
	static void RegisterType(string name, Component* (Entity::*func)(Properties*));

	// Loop Function Overwrites
	virtual void Init();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Destroy();
	virtual void OnDestroy();
	virtual void OnEnable();
	virtual void OnDisable();

	virtual void TriggerEvent(string name) final;

	template<typename... P>
	void TriggerEvent(string name, P... args)
	{
		owner->TriggerEvent(name, args...);
		Object::TriggerEvent(name, args...);
	}

	template<typename... P>
	void TriggerEventWithChildren(string name, P... args)
	{
		TriggerEvent(name, args...);
		owner->TriggerEventWithChildren(name, args...);
	}

	template<typename T>
	void RequireComponent(T*& val)
	{
		ConnectEvent(owner, "SetComponent"+RemoveScoping(Type(*val)), val);
		if(T* t = owner->GetComponent<T>())
		{
			val = t;
			return;
		}
		val = owner->AddComponent<T>();
	}

	template<typename T>
	void RequireComponent(string type, T*& val)
	{
		ConnectEvent(owner, "SetComponent"+type, val);
		if(Component* comp = owner->GetComponent(type))
		{
			val = dynamic_cast<T*>(comp);
			return;
		}
		val = dynamic_cast<T*>(owner->AddComponent(type));
	}

private:

	/// Whether this component should Update or not.
	bool enabled = true;
};

/**	@}*/
/**	@}*/
}
