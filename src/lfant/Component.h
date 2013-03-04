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
#include <lfant/Property.h>
#include <lfant/Entity.h>

namespace lfant
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

//map<string, void (Entity::*)()> componentRegistry;
//#define REGISTER_COMP(comp) static bool BOOST_PP_CAT( comp, __regged ) = componentRegistry.insert( BOOST_PP_STRINGIZE(comp), &Entity::AddComponent<comp>());

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
public:
	virtual ~Component();

	/**
	 *	Returns whether this component is enabled.
	 */
	bool IsEnabled();

	/**
	 *	Enables or disables this component.
	 */
	void SetEnabled(bool enable);

	/// The owner of this Component.
	Entity* owner;

protected:
	Component()
	{
	}
	Component(Entity* owner);

	virtual void Init();

	virtual void Update()
	{
	}

	virtual void Destroy();

	virtual void OnDestroy()
	{
	}

	virtual void OnEnable()
	{
	}

	virtual void OnDisable()
	{
	}

	virtual void OnAddComponent(Component* comp)
	{
	}

	virtual void OnRemoveComponent(Component* comp)
	{
	}

	virtual void Trigger(string name);

	template<typename P1, typename ... P>
	void Trigger(string name, P1 arg, P... args)
	{
		owner->Trigger(name, arg, args...);
		Object::Trigger(name, arg, args...);
	}

private:

	/// Whether this component should Update or not.
	bool enabled = true;
};

/**	@}*/
/**	@}*/
}
