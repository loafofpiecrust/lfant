#ifndef HEADER_F10FF25176ACD9D0
#define HEADER_F10FF25176ACD9D0

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
#include <boost/bind/placeholders.hpp>
#include <boost/function.hpp>
#include <typeinfo>
#include <forward_list>
#include <iostream>

#define SENDER(obj, sig) obj, #sig
#define RECEIVER(obj, slot) obj, &remove_ref<decltype(*obj)>::type::slot

namespace Sqrat {

class Object;
class Function;

}

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
 */
class Object
{
	class Event
	{
	public:
		string name;
		Object* obj = nullptr;

		Event()
		{
		}

		Event(string name, Object* obj = nullptr) :
			name(name), obj(obj)
		{
		}

		virtual ~Event()
		{
		}
	};

	class Event0 : public Event
	{
	public:
		typedef boost::function<void()> funcType;
		funcType func;

		Event0(string name, funcType func, Object* obj) :
			Event(name, obj), func(func)
		{
		}
	};

	template<typename P1, typename ... P>
	class EventP : public Event
	{
	public:
		typedef boost::function<void(P1, P...)> funcType;
		funcType func;

		EventP(string name, funcType func, Object* obj) :
			Event(name, obj), func(func)
		{
		}
	};

	template<typename T>
	class EventVar : public Event
	{
	public:
		T* var;

		EventVar(string name, T* var) :
			Event(name), var(var)
		{
		}
	};

	class EventScript : public Event
	{
	public:
		Sqrat::Function* func;
		Sqrat::Object* obj;

		EventScript(string name, Sqrat::Function* func, Sqrat::Object* obj) :
			Event(name), func(func), obj(obj)
		{
		}
	};

	class Timer
	{
	public:
		string name;
		float time;

		Timer(string name, float time) :
			name(name), time(time)
		{
		}
	};

public:
	Object();
	virtual ~Object();

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
	virtual void Save(Properties* prop) const;

	/**
	 *	Creates a file and calls Save(), outputting to the file.
	 *	@param path File to save to.
	 */
	virtual void SaveFile(string path) const;

	static void Bind() __attribute__((constructor));

	template<typename R>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::*func)(), bool dis = false)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "()";
		for(auto& event : sender->events)
		{
			if(event->name == name && event->obj == receiver)
			{
				delete event;
				event = new Event0(name, boost::bind(func, receiver), receiver);
				receiver->AddSender(sender);
				return;
			}
		}
		if(!dis)
		{
			sender->events.push_back(new Event0(name, boost::bind(func, receiver), receiver));
			receiver->AddSender(sender);
		}
	}

	template<typename R, typename P1>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::* func)(P1), bool dis = false)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "(" + type::Name<P1>() + ")";
		for(auto& event : sender->events)
		{
			if(event->name == name && event->obj == receiver)
			{
				delete event;
				event = new EventP<P1>(name, boost::bind(func, receiver, _1), receiver);
				receiver->AddSender(sender);
				return;
			}
		}
		if(!dis)
		{
			sender->events.push_back(new EventP<P1>(name, boost::bind(func, receiver, _1), receiver));
			receiver->AddSender(sender);
		}
	}

	template<typename R, typename P1, typename P2>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::* func)(P1, P2), bool dis = false)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "(" + type::Name<P1, P2>() + ")";
		for(auto& event : sender->events)
		{
			if(event->name == name && event->obj == receiver)
			{
				delete event;
				event = new EventP<P1, P2>(name, boost::bind(func, receiver, _1, _2), receiver);
				receiver->AddSender(sender);
				return;
			}
		}
		if(!dis)
		{
			sender->events.push_back(new EventP<P1, P2>(name, boost::bind(func, receiver, _1, _2), receiver));
			receiver->AddSender(sender);
		}
	}

	template<typename R, typename P1, typename P2, typename P3>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::* func)(P1, P2, P3), bool dis = false)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "(" + type::Name<P1, P2, P3>() + ")";
		for(auto& event : sender->events)
		{
			if(event->name == name && event->obj == receiver)
			{
				delete event;
				event = new EventP<P1, P2, P3>(name, boost::bind(func, receiver, _1, _2, _3), receiver);
				receiver->AddSender(sender);
				return;
			}
		}
		if(!dis)
		{
			sender->events.push_back(new EventP<P1, P2, P3>(name, boost::bind(func, receiver, _1, _2, _3), receiver));
			receiver->AddSender(sender);
		}
	}

	void ClearEvent(string name)
	{
		name = type::Name(this) + "::" + name;
		for(uint i = 0; i < events.size(); ++i)
		{
			if(events[i]->name == name)
			{
				delete events[i];
				events.erase(events.begin()+i);
			}
		}
	}

	template<typename T>
	static void ConnectEvent(Object* sender, string name, T* var)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "(" + type::Name<T>() + ")";
		EventVar<T>* con = nullptr;
		for(auto& event : sender->events)
		{
			if(event->name == name)
			{
				con = dynamic_cast<EventVar<T>*>(event);
				if(con)
				{
					con->var = var;
					return;
				}
			}
		}
		con = new EventVar<T>(name, var);
		sender->events.push_back(con);
	}

	virtual void TriggerEvent(string name)
	{
		erase_all(name, " ");
		name = type::Name(this) + "::" + name + "()";
		for(auto& event : events)
		{
			if(event->name == name)
			{
				Event0* con = dynamic_cast<Event0*>(event);
				if(con)
				{
					con->func();
				}
			}
		}
	}

	template<typename P1>
	void TriggerEvent(string name, P1 arg)
	{
		erase_all(name, " ");
		string name2 = type::Name(this) + "::" + name + "(" + type::Name<P1>() + ")";
		for(auto& event : events)
		{
			if(event->name == name2)
			{
				EventP<P1>* con = dynamic_cast<EventP<P1>*>(event);
				if(con)
				{
					con->func(arg);
				//	continue;
				}
				EventVar<P1>* conv = dynamic_cast<EventVar<P1>*>(event);
				if(conv)
				{
					*conv->var = arg;
				//	continue;
				}
			}
		}
		TriggerEvent(name);
	}

	template<typename P1, typename P2>
	void TriggerEvent(string name, P1 arg, P2 arg2)
	{
		erase_all(name, " ");
		string name2 = type::Name(this) + "::" + name + "(" + type::Name<P1, P2>() + ")";
		for(auto& event : events)
		{
			if(event->name == name2)
			{
				auto con = dynamic_cast<EventP<P1, P2>*>(event);
				if(con)
				{
					con->func(arg, arg2);
				}
			}
		}
		TriggerEvent(name, arg);
	}

	template<typename P1, typename P2, typename P3>
	void TriggerEvent(string name, P1 arg, P2 arg2, P3 arg3)
	{
		erase_all(name, " ");
		string name2 = type::Name(this) + "::" + name + "(" + type::Name<P1, P2, P3>() + ")";
		for(auto& event : events)
		{
			if(event->name == name2)
			{
				auto con = dynamic_cast<EventP<P1, P2, P3>*>(event);
				if(con)
				{
					con->func(arg, arg2, arg3);
				}
			}
		}
		TriggerEvent(name, arg, arg2);
	}

	bool EventConnected(string name)
	{
		erase_all(name, " ");
		name = type::Name(this) + "::" + name;
		for(auto& event : events)
		{
			if(event->name == name)
			{
				return true;
			}
		}
		return false;
	}

	void SetTimer(string name, float time)
	{
		erase_all(name, " ");
	//	name = type::Name(this) + "::" + name + "()";
		for(auto& t : timers)
		{
			if(t->name == name)
			{
				t->time = time;
				return;
			}
		}
		timers.push_back(new Timer(name, time));
	}

	void CancelTimer(string name)
	{
		erase_all(name, " ");
	//	name = type::Name(this) + "::" + name + "()";
		for(uint i = 0; i < timers.size(); ++i)
		{
			if(timers[i]->name == name)
			{
				timers.erase(timers.begin()+i);
				return;
			}
		}
	}

	float* GetTimer(string name)
	{
		erase_all(name, " ");
	//	name = type::Name(this) + "::" + name + "()";
		for(auto& t : timers)
		{
			if(t->name == name)
			{
				return &t->time;
			}
		}
		return nullptr;
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

	virtual void Render();

	virtual void FixedUpdate();

	/**
	 *	Call this to destroy the object.
	 */
	virtual void Destroy();

protected:
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
	virtual void Deinit();

	bool initBeforeLoad = false;

private:
	// For scripts
	void ConnectScriptEvent(Object* sender, string name, Sqrat::Object* receiver, Sqrat::Function* func)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "()";
		EventScript* con = nullptr;
		for(auto& event : sender->events)
		{
			if(event->name == name)
			{
				con = dynamic_cast<EventScript*>(event);
				if(con)
				{
					con->func = func;
					con->obj = receiver;
					return;
				}
			}
		}
		con = new EventScript(name, func, receiver);
		sender->events.push_back(con);
	}

	void AddSender(Object* sender)
	{
		for(auto& s : senders)
		{
			if(s == sender)
			{
				return;
			}
		}
		senders.push_back(sender);
	}

	deque<Event*> events;
	deque<Object*> senders;
	deque<Timer*> timers;

};

/// @}
/// @}

} /* namespace lfant */

#endif // header guard
