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
#include <lfant/FileSystem.h>

// External
#include <sqrat/sqratObject.h>
#include <sqrat/sqratFunction.h>

namespace lfant
{

//decltype(Object::events) Object::events;


Object::Object()
{
}

Object::~Object()
{
/*	for(auto i = events.begin(); i != events.end(); ++i)
	{
		if((*i)->sender == this)
		{
			events.erase(i);
			--i;
		}
	}*/
/*	for(int i = 0; i < events.size(); ++i)
	{
		if(events[i]->sender == this)
		{
			events.erase(events.begin()+i);
			--i;
		}
	}*/
	for(auto& con : eventConnections)
	{
		con.disconnect();
	}
}

uint32 Object::GetEventCount()
{
	return events.size();
}

void Object::Init()
{
}

void Object::Update()
{
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


	// @todo Keep?
//	delete this;
}

void Object::Deinit()
{
}

void Object::LoadFile(string path)
{
	if(path.empty()) return;

//	GetGame()->Log(type::Name(this), " loading file '", path, "'.");
	auto rp = GetGame()->GetAssetPath(path);
	if(!fs::exists(rp)) return;

	Properties prop;
	prop.LoadFile(rp.string());
	prop.SetMode(Properties::Mode::Input);

	std::cout << "\nchildren we have " << prop.GetChildCount() << " in " << &prop << "\n";

//	string type = type::Descope(type::Name(this));
//	to_lower(type);
//	GetGame()->Log("Checking for first child");
	if(Properties* pc = prop.GetChild(0))
	{
	//	GetGame()->Log("Loading first child");
		std::cout << "\nMOAR CHILDS: " << pc << " " << pc->GetChildCount() << "\n";
		Serialize(pc);
	}
//	GetGame()->Log("Done loading file");
}

void Object::Serialize(Properties *prop)
{
}

void Object::SaveFile(string path)
{
	Properties prop;
	prop.SetMode(Properties::Mode::Output);
	Serialize(&prop);
	prop.SaveFile(path);
}

void Object::ScriptBind()
{
	Script::ClassBase<Object, Sqrat::Class<Object, Sqrat::NoCopy<Object>>> inst;
//	Script::BaseClass<Object> inst;

	inst.Func("Init", &Object::Init);
	inst.Func("Update", &Object::Update);
	inst.Func("Destroy", &Object::Destroy);
	inst.Func("Deinit", &Object::Deinit);
	inst.Func("ConnectEvent", &Object::ConnectScriptEvent);
	inst.Func("TriggerEvent", (void (Object::*)(string))&Object::TriggerEvent);
//	inst.Func("SetTimer", &Object::SetTimer);
//	inst.Func("CancelTimer", &Object::CancelTimer);
//	inst.Func("GetTimer", &Object::GetTimer);
	inst.Func("Serialize", &Object::Serialize);
//	inst.Func("Save", &Object::Save);
	inst.Func("LoadFile", &Object::LoadFile);
	inst.Func("SaveFile", &Object::SaveFile);

	inst.Bind();
}

void Object::ConnectScriptEvent(Object* sender, string name, Sqrat::Object receiver, string funcName)
{
	Sqrat::Function func (receiver, funcName.c_str());

	erase_all(name, " ");
//	name = type::Name(sender) + "::" + name;
	EventScript* con = nullptr;
	for(auto& evt : sender->events)
	{
		if(evt.first == name && typeid(*evt.second) == typeid(EventScript))
		{
			con = static_cast<EventScript*>(evt.second.get());
			con->Register(func);
			return;
		}
	}
	con = new EventScript();
	con->Register(func);
	sender->events[name] = con;
}

}
