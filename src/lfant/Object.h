/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-28 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
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

#define BOOST_BIND_NO_PLACEHOLDERS

// Internal
#include <lfant/TypeInfo.h>
#include <lfant/Properties.h>

// External
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <typeinfo>
#include <forward_list>
#include <iostream>

#define SENDER(obj, sig) obj, #sig
#define RECEIVER(obj, slot) obj, &remove_ref<decltype(*obj)>::type::slot

namespace lfant
{

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *	The base class for any object-oriented classes.
 *	Implements a signals and slots system similar to Qt's. Also contains
 *	a few basic game-loop functions to allow homogeneous pointers to an Object
 *	instance, like in ActionScript.
 *	@todo
 *		Possibly make the connection naming more efficient.
 *		Be sure that the variadic templating works.
 */
class Object
{
	class Event
	{
	public:
		string name;

		Event()
		{
		}

		Event(string name) :
			name(name)
		{
		}

		virtual ~Event()
		{
		}
	};

	class Event0 : public Event
	{
	public:
		typedef boost::signals2::signal<void ()> sigType;
		sigType sig;

		Event0(string name, boost::function<void()> func) :
			Event(name)
		{
			sig.connect(func);
		}
	};

	template<typename P1, typename ... P>
	class EventP : public Event
	{
	public:
		typedef boost::signals2::signal<void (P1, P ...)> sigType;
		sigType sig;

		EventP(string name, boost::function<void(P1, P ...)> func) :
			Event(name)
		{
			sig.connect(func);
		}
	};
public:

	/**
	 *	Loads the object's data from a Properties structure.
	 *	@param prop The Properties namespace to load from.
	 */
	virtual void Load(Properties* prop);

	/**
	 *	Loads a Properties structure from a file, then calling Load().
	 *	@param path File to load from.
	 */
	virtual void LoadFile(string path);

	/**
	 *	Saves Object data to a Properties structure.
	 *	@param prop Namespace to save to.
	 */
	virtual void Save(Properties* prop);

	/**
	 *	Creates a file and calls Save(), outputting to the file.
	 *	@param path File to save to.
	 */
	virtual void SaveFile(string path);

	template<typename R>
	void ConnectEvent(Object* sender, string name, R* receiver, void (R::* func)())
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "()";
		Event0* con = nullptr;
		for(auto& event : sender->events)
		{
			if(event->name == name)
			{
				con = dynamic_cast<Event0*>(event.get());
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver));
				}
				return;
			}
		}
		con = new Event0(name, boost::bind(func, receiver));
		sender->events.push_front(ptr<Event>(con));
	}

	template<typename R, typename P1>
	void ConnectEvent(Object* sender, string name, R* receiver, void (R::* func)(P1))
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "(" + Type<P1>() + ")";
		EventP<P1>* con = nullptr;
		for(auto& event : sender->events)
		{
			if(event->name == name)
			{
				con = dynamic_cast<EventP<P1>*>(event.get());
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver, _1));
				}
				return;
			}
		}
		con = new EventP<P1>(name, boost::bind(func, receiver, _1));
		sender->events.push_front(ptr<Event>(con));
	}

	template<typename R, typename P1, typename P2>
	void ConnectEvent(Object* sender, string name, R* receiver, void (R::* func)(P1, P2))
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "(" + Type<P1, P2>() + ")";
		EventP<P1, P2>* con = nullptr;
		for(auto& event : sender->events)
		{
			if(event->name == name)
			{
				con = dynamic_cast<EventP<P1, P2>*>(event.get());
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver, _1, _2));
				}
				return;
			}
		}
		con = new EventP<P1, P2>(name, boost::bind(func, receiver, _1, _2));
		sender->events.push_front(ptr<Event>(con));
	}

	template<typename R, typename P1, typename P2, typename P3>
	void ConnectEvent(Object* sender, string name, R* receiver, void (R::* func)(P1, P2, P3))
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "(" + Type<P1, P2, P3>() + ")";
		EventP<P1, P2, P3>* con = nullptr;
		for(auto& event : sender->events)
		{
			if(event->name == name)
			{
				con = dynamic_cast<EventP<P1, P2, P3>*>(event.get());
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver, _1, _2, _3));
				}
				return;
			}
		}
		con = new EventP<P1, P2, P3>(name, boost::bind(func, receiver, _1, _2, _3));
		sender->events.push_front(ptr<Event>(con));
	}


	virtual void TriggerEvent(string name)
	{
		erase_all(name, " ");
		string type = Type(this) + "::" + name + "()";
		for(auto& event : events)
		{
			if(event->name == type)
			{
				Event0* con = dynamic_cast<Event0*>(event.get());
				if(con)
				{
					con->sig();
				}
			}
		}
	}

	template<typename P1>
	void TriggerEvent(string name, P1 arg)
	{
		erase_all(name, " ");
		string type = Type(this) + "::" + name + "(" + Type<P1>() + ")";
		for(auto& event : events)
		{
			if(event->name == type)
			{
				auto con = dynamic_cast<EventP<P1>*>(event.get());
				if(con)
				{
					con->sig(arg);
				}
			}
		}
		TriggerEvent(name);
	}

	template<typename P1, typename P2>
	void TriggerEvent(string name, P1 arg, P2 arg2)
	{
		erase_all(name, " ");
		string type = Type(this) + "::" + name + "(" + Type<P1, P2>() + ")";
		for(auto& event : events)
		{
			if(event->name == type)
			{
				auto con = dynamic_cast<EventP<P1, P2>*>(event.get());
				if(con)
				{
					con->sig(arg, arg2);
				}
			}
		}
		TriggerEvent(name, arg);
	}

	template<typename P1, typename P2, typename P3>
	void TriggerEvent(string name, P1 arg, P2 arg2, P3 arg3)
	{
		erase_all(name, " ");
		string type = Type(this) + "::" + name + "(" + Type<P1, P2, P3>() + ")";
		for(auto& event : events)
		{
			if(event->name == type)
			{
				auto con = dynamic_cast<EventP<P1, P2, P3>*>(event.get());
				if(con)
				{
					con->sig(arg, arg2, arg3);
				}
			}
		}
		TriggerEvent(name, arg, arg2);
	}

	bool ConnectEvented(string name)
	{
		erase_all(name, " ");
		name = Type(this) + "::" + name;
		for(auto& event : events)
		{
			if(event->name == name)
			{
				return true;
			}
		}
		return false;
	}

	/**
	 *	Called when the object is initialised. Used instead of
	 *	constructor to ensure that all subsystems are created as well.
	 */
	virtual void Init();

	/**
	 *	Called every frame to update the object.
	 */
	virtual void Update();

	/**
	 *	Call this to destroy the object.
	 */
	virtual void Destroy();

protected:
	Object();
	virtual ~Object();

	struct Delete
	{
		void operator()(Object* obj)
		{
		//	obj->Destroy();
			delete obj;
		}
	};

	/**
	 *	Called just before the object is destroyed.
	 */
	virtual void OnDestroy();

	/**
	 *	Called when the object should be bound to script.
	 */
	virtual void Bind();

private:

	deque< ptr<Event> > events;
};

/// @}
/// @}

} /* namespace lfant */
