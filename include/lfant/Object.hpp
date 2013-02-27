/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include <lfant/stdafx.hpp>

#define BOOST_BIND_NO_PLACEHOLDERS

// External
#include <boost/signals2.hpp>
#include <typeinfo>

// Internal
#include <lfant/TypeInfo.hpp>
#include <lfant/Property.hpp>

#define SENDER(obj, sig) obj, #sig
#define RECEIVER(obj, slot) obj, &remove_ref<decltype(*obj)>::type::slot

namespace lfant
{

/** @addtogroup Engine
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
		Object* ptr;

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
		typedef boost::signals2::signal<void()> sigType;
		sigType sig;

		Event0(string name, boost::function<void()> func) :
			Event(name)
		{
			sig.connect(func);
		}
	};

	template<typename P1, typename... P>
	class EventP : public Event
	{
	public:
		typedef boost::signals2::signal<void(P1, P...)> sigType;
		sigType sig;

		EventP(string name, boost::function<void(P1, P...)> func) :
			Event(name)
		{
			sig.connect(func);
		}
	};
public:

	template<typename R>
	void Connect(Object* sender, string name, R* receiver, void (R::*func)())
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "()";
		cout << "Connecting "+name+"\n";
		Event0* con = nullptr;
		for(auto event : sender->events)
		{
			if(event->name == name)
			{
				con = dynamic_cast<Event0*>(event);
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver));
					con->ptr = sender;
				}
				return;
			}
		}
		con = new Event0(name, boost::bind(func, receiver));
		con->ptr = sender;
		sender->events.push_back(con);
	}

	template<typename R, typename P1>
	void Connect(Object* sender, string name, R* receiver, void (R::*func)(P1))
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "(" + Type<P1>() + ")";
		cout << "Connecting "+name+"\n";
		EventP<P1>* con = nullptr;
		for(auto connection : sender->events)
		{
			if(connection->name == name)
			{
				con = dynamic_cast<EventP<P1>*>(connection);
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver, _1));
					con->ptr = sender;
				}
				return;
			}
		}
		con = new EventP<P1>(name, boost::bind(func, receiver, _1));
		sender->events.push_back(con);
	}

	template<typename R, typename P1, typename P2>
	void Connect(Object* sender, string name, R* receiver, void (R::*func)(P1, P2))
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "(" + Type<P1, P2>() + ")";
		cout << "Connecting "+name+"\n";
		EventP<P1, P2>* con = nullptr;
		for(auto connection : sender->events)
		{
			if(connection->name == name)
			{
				con = dynamic_cast<EventP<P1, P2>*>(connection);
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver, _1, _2));
					con->ptr = sender;
				}
				return;
			}
		}
		con = new EventP<P1, P2>(name, boost::bind(func, receiver, _1, _2));
		sender->events.push_back(con);
	}

	template<typename R, typename P1, typename P2, typename P3>
	void Connect(Object* sender, string name, R* receiver, void (R::*func)(P1, P2, P3))
	{
		erase_all(name, " ");
		name = Type(sender) + "::" + name + "(" + Type<P1, P2, P3>() + ")";
		cout << "Connecting "+name+"\n";
		EventP<P1, P2, P3>* con = nullptr;
		for(auto connection : sender->events)
		{
			if(connection->name == name)
			{
				con = dynamic_cast<EventP<P1, P2, P3>*>(connection);
				if(con)
				{
					con->sig.connect(boost::bind(func, receiver, _1, _2, _3));
					con->ptr = sender;
				}
				return;
			}
		}
		con = new EventP<P1, P2, P3>(name, boost::bind(func, receiver, _1, _2, _3));
		sender->events.push_back(con);
	}


	virtual void Trigger(string name)
	{
		erase_all(name, " ");
		name = Type(this) + "::" + name + "()";
		cout << "Triggering "+name+"\n";
		for(auto event : events)
		{
			if(event->name == name && event->ptr == this)
			{
				Event0* con = dynamic_cast<Event0*>(event);
				if(con)
				{
					con->sig();
				}
			}
		}
	}

	template<typename P1, typename ... P>
	void Trigger(string name, P1 arg, P ... args)
	{
		erase_all(name, " ");
		name = Type(this) + "::" + name + "(" + Type<P1, P...>() + ")";
		cout << "Triggering "+name+"\n";
		for(auto event : events)
		{
			if(event->name == name && event->ptr == this)
			{
				auto con = dynamic_cast<EventP<P1, P...>*>(event);
				if(con)
				{
					con->sig(arg, args...);
				}
			}
		}
		//Trigger(name);
	}

	bool Connected(string name)
	{
		erase_all(name, " ");
		name = Type(this) + "::" + name;
		for(uint i = 0; i < events.size(); ++i)
		{
			if(events[i]->name == name)
			{
				return true;
			}
		}
		return false;
	}

	virtual void Init();
	virtual void Update()
	{
	}
	virtual void Destroy();

protected:
	Object();
	virtual ~Object();

	virtual void Destroy(Object* obj);
	virtual void OnDestroy()
	{
	}
	virtual void Bind();

private:
	vector<Event*> events;
};

/// @}
/// @}

} /* namespace lfant */