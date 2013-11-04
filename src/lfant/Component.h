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

// Internal
#include <lfant/util/qumap.h>
#include <lfant/Object.h>
#include <lfant/Entity.h>
#include <lfant/TypeRegistry.h>

// External

namespace lfant {

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */


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
//	friend class lfant::editor::gui::Window;
	friend class Entity;
	DECLARE_REGISTRY(Component)
public:
	Component();
	Component(const Component& other);
	virtual ~Component();

//	virtual Component& operator=(const Component& other);

	virtual Component* Clone(Entity* owner) const;
	void Clone(Component* comp, Entity* owner) const;

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop) const;

	virtual void Destroy() final;

	static void Bind() __attribute__((constructor));

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

	// Loop Function Overwrites
	virtual void Init();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Render();
	virtual void Deinit(); // Rename to OnDestroy()?

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
		ConnectEvent(owner, "OnSetComponent"+RemoveScoping(Type(*val)), val);
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
		ConnectEvent(owner, "OnSetComponent"+type, val);
		if(Component* comp = owner->GetComponent(type))
		{
			val = dynamic_cast<T*>(comp);
			return;
		}
		val = dynamic_cast<T*>(owner->AddComponent(type));
	}

private:

//	static TypeRegistry<Component> registry;

	/// Whether this component should Update or not.
	bool enabled = true;
};

/**	@}*/
/**	@}*/
}
