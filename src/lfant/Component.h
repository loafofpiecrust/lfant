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
public:
	DECLARE_REGISTRY(Component)

	Component();
	Component(const Component& other);
	virtual ~Component();

//	virtual Component& operator=(const Component& other);

	/// @todo Remove or fix Clone()
	virtual Component* Clone(Entity* owner) const;
	void Clone(Component* comp, Entity* owner) const;

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop) const;

	virtual void Destroy() final;

	/// @todo Rename Bind() to something clearer
	static void ScriptBind();

	/**
	 *	Returns whether this component is enabled.
	 */
	bool IsEnabled();

	/**
	 *	Enables or disables this component.
	 */
	virtual void Enable(bool enable = true);

	void SetOwner(Entity* ent);
	Entity* GetOwner() const;

	/// The owner of this Component.
	Entity* owner = nullptr;

protected:

	// Loop Function Overwrites
	virtual void Init();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Render();
	virtual void Deinit(); // Rename to OnDestroy()?

//	virtual void OnEnable();
//	virtual void OnDisable();

	virtual void TriggerEvent(string name) final;

	// @todo Centralize these special TriggerEvent() overloads
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
	void ConnectComponent(T*& val, bool required = false)
	{
		ConnectEvent(owner, "SetComponent"+type::Descope(type::Name<T>()), (Component**)&val);
		val = owner->GetComponent<T>();
		if(!val && required)
		{
			val = new T;
			owner->AddComponent(val);
		}
	}

	template<typename T>
	void ConnectComponent(string type, T*& val, bool required = false)
	{
		ConnectEvent(owner, "SetComponent"+type, (Component**)&val);
		val = dynamic_cast<T*>(owner->GetComponent(type));
		if(!val && required)
		{
			val = dynamic_cast<T*>(Component::NewFromString(type));
			owner->AddComponent(val);
		}
	}

private:
	/// Whether this component should Update or not.
	bool enabled = true;
};

/**	@}*/
/**	@}*/
}
