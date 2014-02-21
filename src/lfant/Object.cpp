/*
*	Copyright (C) 2013-2014, by loafofpiecrust
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

#include <lfant/Object.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Console.h>
#include <lfant/ScriptSystem.h>

// External
#include <sqrat/sqratObject.h>
#include <sqrat/sqratFunction.h>

namespace lfant
{

std::deque<ptr<Object::EventBase>> Object::events;

Object::Object()
{
}

Object::~Object()
{
}

uint32_t Object::GetEventCount()
{
	return Object::events.size();
}

void Object::Init()
{
	Bind();
}

void Object::Update()
{
	for(uint i = 0; i < timers.size(); ++i)
	{
	//	Log("Timer '", timers[i]->name, "' updated at ", timers[i]->time);
		if(timers[i].time <= 0.0f)
		{
			string name = timers[i].name;
			timers.erase(timers.begin()+i);
			--i;
		//	Log("Timer triggered");
			TriggerEvent(name);
			continue;
		}
		timers[i].time -= game->time->deltaTime;
	}
}

void Object::Render()
{
}

void Object::FixedUpdate()
{

}

void Object::Destroy()
{
	Deinit();

	for(uint i = 0; i < Object::events.size(); ++i)
	{
		if(events[i]->sender == this)
		{
			events[i].reset();
			events.erase(events.begin()+i);
		}
	}

	// @todo Keep?
//	delete this;
}

void Object::Deinit()
{
}

void Object::LoadFile(string path)
{
	Log(type::Name(this), " loading file '", path, "'.");
	Properties prop;
	prop.LoadFile(path);

//	string type = type::Descope(type::Name(this));
//	to_lower(type);
	Log("Checking for first child");
	if(Properties* pc = prop.GetFirstChild())
	{
		Log("Loading first child");
		Load(pc);
	}
	Log("Done loading file");
}

void Object::Load(Properties *prop)
{
}

void Object::SaveFile(string path) const
{
	Properties prop;
	Save(&prop);
	prop.SaveFile(path);
}

void Object::Save(Properties *prop) const
{
/*	string type = type::Descope(type::Name(this));
	to_lower(type);
	prop->SetName(type);*/
}

void Object::Bind()
{
	/*
	Script::BaseClass<Object> inst;

	inst.Func("Init", &Object::Init);
	inst.Func("Update", &Object::Update);
	inst.Func("Destroy", &Object::Destroy);
	inst.Func("Deinit", &Object::Deinit);
	inst.Func("ConnectEvent", &Object::ConnectScriptEvent);
	inst.Func("SetTimer", &Object::SetTimer);
	inst.Func("CancelTimer", &Object::CancelTimer);
	inst.Func("GetTimer", &Object::GetTimer);
	inst.Func("Load", &Object::Load);
	inst.Func("Save", &Object::Save);
	inst.Func("LoadFile", &Object::LoadFile);
	inst.Func("SaveFile", &Object::SaveFile);
	*/
}

void Object::ConnectScriptEvent(Object* sender, string name, Sqrat::Object* receiver, Sqrat::Function* func)
{
	boost::algorithm::erase_all(name, " ");
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

bool Object::EventConnected(string name)
{
//	erase_all(name, " ");
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

void Object::SetTimer(string name, float time)
{
//	erase_all(name, " ");
//	name = type::Name(this) + "::" + name + "()";
	for(auto& t : timers)
	{
		if(t.name == name)
		{
			t.time = time;
			return;
		}
	}
	timers.emplace_back(name, time);
}

void Object::CancelTimer(string name)
{
//	erase_all(name, " ");
//	name = type::Name(this) + "::" + name + "()";
	for(uint i = 0; i < timers.size(); ++i)
	{
		if(timers[i].name == name)
		{
			timers.erase(timers.begin()+i);
			return;
		}
	}
}

float* Object::GetTimer(string name)
{
//	erase_all(name, " ");
//	name = type::Name(this) + "::" + name + "()";
	for(auto& t : timers)
	{
		if(t.name == name)
		{
			return &(t.time);
		}
	}
	return nullptr;
}

}
