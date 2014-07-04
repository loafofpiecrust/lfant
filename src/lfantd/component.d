
module lfant.component;

/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

import std.stdio;

// Internal
import lfant.gameobject;
import lfant.entity;
import lfant.game;

// External

class Component : GameObject
{
public:
//	DECLARE_REGISTRY(Component)

	this() {}
	~this()
	{
		writeln("component");
	}

	/// @todo Remove or fix Clone()
//	Component* Clone(Entity* owner) const;
//	void Clone(Component* comp, Entity* owner) const;

//	void Serialize(Properties* prop);

	override void initialize()
	{
	}

	final void destroy()
	{
	}

	/// @todo Rename Bind() to something clearer
//	static void ScriptBind();

	@property
	{
		bool rendered() const 
		{
			return _rendered;
		}
	
		void owner(Entity ent)
		{
			
		}
		Entity owner() nothrow
		{
			return _owner;
		}
	
		override Game game() nothrow
		{
			return owner.game;
		}
	}

	/// Whether this component should Update or not.
	bool enabled = true;

protected:

	// Loop Function Overwrites
	override void render() {};

//	final void TriggerEvent(string name);

	// @todo Centralize these special TriggerEvent() overloads
//	template<typename... P>
	void triggerEvent(P...)(string name, P args)
	{
		owner.triggerEvent(name, args);
	//	Object.TriggerEvent(name, args);
	}

//	template<typename... P>
	void triggerEventWithChildren(P...)(string name, P args)
	{
		triggerEvent(name, args);
		owner.triggerEventWithChildren(name, args);
	}

//	template<typename T>
	void connectComponent(T)(ref T val)
	{
	//	ConnectEvent(owner, "SetComponent"+type::Descope(type::Name<T>()), (Component**)&val);
		val = owner.getComponent(T)();
	}

//	template<typename T>
	void connectComponent(T)(string type, ref T val)
	{
		connectEvent(owner, "SetComponent"+type, val);
		val = cast(T*)(owner.getComponent(type));
	}

//	template<typename T>
	void requireComponent(T)(ref T val)
	{
		connectComponent(val);
		if(!val)
		{
			val = new T();
			owner.addComponent(val);
		}
	}

	bool _rendered = false;

private:
	/// The owner of this Component.
	Entity _owner = null;
};
