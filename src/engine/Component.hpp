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

// Internal
#include "Object.hpp"
#include "Transform.hpp"
#include "Property.hpp"
#include "Entity.hpp"

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

/**	The base class for all Entity Components.
 *	@details
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
public:
	Component(Entity* owner);

	/**
	 *	Destroys this Component instance.
	 */
	void Destroy();

	/// The owner of this Component.
	Entity* owner;

	// Default components
	Transform* transform;

protected:
	friend class Entity;
	Component()
	{
	}
	virtual ~Component();

	virtual void Init()
	{
	}

	virtual void Update()
	{
	}

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

private:
	/**
	 *	Returns whether this component is enabled.
	 */
	bool GetEnabled();

	/**
	 *	Enables or disables this component.
	 */
	void SetEnabled(bool enable);

	/// Whether this component should Update or not.
	bool _enabled = true;

public:

	// Properties
	PROP_RW(Component, enabled, GetEnabled, SetEnabled)
};

/**	@}*/
/**	@}*/
}
