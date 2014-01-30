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
	class EventBase
	{
	public:
		string name = "";
		Object* sender = nullptr;

		virtual void Clear() {}

		EventBase()
		{
		}

		EventBase(string name, Object* sender = nullptr) :
			name(name), sender(sender)
		{
		}

		virtual ~EventBase()
		{
		}
	};

/*	class Event0 : public Event
	{
	public:
		typedef boost::function<void()> funcType;
		deque<funcType> func;

		Event0(string name, Object* sender) :
			Event(name, sender)
		{
		}
	};*/

	class Event0 : public EventBase
	{
		typedef boost::function<void()> funcType;
		deque<funcType> functions;

		Event0(string name, Object* sender) :
			EventBase(name, sender)
		{
		}

		virtual ~Event0() {}

		void Register(funcType func)
		{
			functions.push_back(func);
		}

		bool IsRegistered(funcType func)
		{
			for(funcType f : functions)
			{
//				if(f == func)
				{
	//				return true;
				}
			}
		}

		void Clear()
		{
			functions.clear();
		}

		void Trigger()
		{
			for(auto& func : functions)
			{
				func();
			}
		}
	};

	template<typename ... P>
	class Event : public EventBase
	{
	public:
		typedef boost::function<void(P...)> funcType;
		deque<funcType> functions;

		Event(string name, Object* sender) :
			EventBase(name, sender)
		{
		}

		virtual ~Event() {}

		void Register(funcType func)
		{
			functions.push_back(func);
		}

		bool IsRegistered(funcType func)
		{
			for(auto& f : functions)
			{
				if(f == func)
				{
					return true;
				}
			}
		}

		void Clear()
		{
			functions.clear();
		}

		void Trigger(P... args)
		{
			for(auto& func : functions)
			{
				func(args...);
			}
		}
	};

	template<typename T>
	class EventVar : public EventBase
	{
	public:
		T* var;

		EventVar(string name, T* var) :
			EventBase(name), var(var)
		{
		}
	};

	class EventScript : public EventBase
	{
	public:
		Sqrat::Function* func;
		Sqrat::Object* obj;

		EventScript(string name, Sqrat::Function* func, Sqrat::Object* obj) :
			EventBase(name), func(func), obj(obj)
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

	template<typename C>
	static void RegisterEventFunc(Event<>* evt, C* receiver, void (C::*func)())
	{
		evt->Register(boost::bind(func, receiver));
	}

	template<typename C, typename P1>
	static void RegisterEventFunc(Event<P1>* evt, C* receiver, void (C::*func)(P1))
	{
		evt->Register(boost::bind(func, receiver, _1));
	}

	template<typename C, typename P1, typename P2>
	static void RegisterEventFunc(Event<P1,P2>* evt, C* receiver, void (C::*func)(P1,P2))
	{
		evt->Register(boost::bind(func, receiver, _1, _2));
	}

	template<typename C, typename P1, typename P2, typename P3>
	static void RegisterEventFunc(Event<P1,P2,P3>* evt, C* receiver, void (C::*func)(P1,P2,P3))
	{
		evt->Register(boost::bind(func, receiver, _1, _2, _3));
	}

	template<typename C, typename P1, typename P2, typename P3, typename P4>
	static void RegisterEventFunc(Event<P1,P2,P3,P4>* evt, C* receiver, void (C::*func)(P1,P2,P3,P4))
	{
		evt->Register(boost::bind(func, receiver, _1, _2, _3, _4));
	}

	template<typename R>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::*func)(), bool dis = false)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "()";
		for(auto& evt : Object::events)
		{
			if(evt->name == name && evt->sender == sender)
			{
				Event<>* evtp = dynamic_cast<Event<>*>(evt.get());
				RegisterEventFunc<R>(evtp, receiver, func);
				return;
			}
		}
		if(!dis)
		{
			Event<>* evt = new Event<>(name, sender);
			Object::events.push_back(evt);
			RegisterEventFunc<R>(new Event<>(name, sender), receiver, func);
		}
	}

	template<typename R, typename P1, typename... P>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::*func)(P1, P...), bool dis = false)
	{
		erase_all(name, " ");
		name = type::Name<Object*>(sender) + "::" + name + "(" + type::Name<P1, P...>() + ")";
		for(auto& evt : Object::events)
		{
			if(evt->name == name && evt->sender == sender)
			{
				Event<P1, P...>* evtp = dynamic_cast<Event<P1, P...>*>(evt.get());
				RegisterEventFunc<R, P1, P...>(evtp, receiver, func);
				return;
			}
		}
		if(!dis)
		{
			Event<P1, P...>* evt = new Event<P1, P...>(name, sender);
			Object::events.push_back(evt);
			RegisterEventFunc<R, P1, P...>(evt, receiver, func);
		}
	}

	template<typename C>
	static void ClearEvent(string name)
	{
		name = type::Name<C>() + "::" + name;
		for(auto& evt : events)
		{
			if(evt->name == name)
			{
				evt->Clear();
			}
		}
	}

	template<typename T>
	static void ConnectEvent(Object* sender, string name, T* var)
	{
		erase_all(name, " ");
		name = type::Name(sender) + "::" + name + "(" + type::Name<T>() + ")";
		EventVar<T>* con = nullptr;
		for(auto& evt : Object::events)
		{
			if(evt->name == name)
			{
				con = dynamic_cast<EventVar<T>*>(evt.get());
				if(con)
				{
					con->var = var;
					return;
				}
			}
		}
		con = new EventVar<T>(name, var);
		Object::events.push_back(con);
	}

	void TriggerEvent(string name)
	{
		erase_all(name, " ");
		name = type::Name(this) + "::" + name + "()";
		for(auto& evt : Object::events)
		{
			if(evt->sender == this && evt->name == name)
			{
				Event<>* con = dynamic_cast<Event<>*>(evt.get());
				con->Trigger();
				return;
			}
		}

	}

	template<typename P1>
	void TriggerEvent(string name, P1 arg)
	{
		erase_all(name, " ");
		name = type::Name(this) + "::" + name + "(" + type::Name<P1>() + ")";
		for(auto& evt : Object::events)
		{
			if(evt->sender == this && evt->name == name)
			{
				Event<P1>* con = dynamic_cast<Event<P1>*>(evt.get());
				if(con)
				{
					con->Trigger(arg);
				}
				else
				{
					EventVar<P1>* conv = dynamic_cast<EventVar<P1>*>(evt.get());
					if(conv)
					{
						*conv->var = arg;
					}
				}
				return;
			}
		}
	}


	template<typename P1, typename... P>
	void TriggerEvent(string name, P1 arg1, P... args)
	{
		erase_all(name, " ");
		name = type::Name(this) + "::" + name + "(" + type::Name<P1, P...>() + ")";
		for(auto& evt : Object::events)
		{
			if(evt->sender == this && evt->name == name)
			{
				Event<P1, P...>* con = dynamic_cast<Event<P1, P...>*>(evt.get());
				if(con)
				{
					con->Trigger(arg1, args...);
				}
				return;
			}
		}
	}
/*
	template<typename P1, typename PL>
	virtual void TriggerEvent(string name, P1 arg1, PL argLast)
	{
		erase_all(name, " ");
		name = type::Name(this) + "::" + name + "(" + type::Name<P1, PL>() + ")";
		for(auto& evt : Object::events)
		{
			if(evt->sender == this && evt->name == name)
			{
				Event<P1, PL>* con = dynamic_cast<Event<P1, PL>*>(evt);
				if(con)
				{
					con->Trigger(arg1, argLast);
				}
				return;
			}
		}
	}

	template<typename P1, typename PL, typename... P>
	virtual void TriggerEvent(string name, P1 arg1, P... args, PL argLast)
	{
		erase_all(name, " ");
		name = type::Name(this) + "::" + name + "(" + type::Name<P1, P...>() + ")";
		for(auto& evt : Object::events)
		{
			if(evt->sender == this && evt->name == name)
			{
				Event<P1, P...>* con = dynamic_cast<Event<P1, P...>*>(evt);
				if(con)
				{
					con->Trigger(arg1, args..., argLast);
				}
				return;
			}
		}
	}
*/

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
				con = dynamic_cast<EventScript*>(event.get());
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

	static deque<ptr<EventBase>> events;
	deque<Timer*> timers;

};

/// @}
/// @}

} /* namespace lfant */
