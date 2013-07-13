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

#include <lfant/Object.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Console.h>
#include <lfant/ScriptSystem.h>

// External

namespace lfant
{

Object::Object()
{
}

Object::~Object()
{
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
		if(timers[i]->time <= 0.0f)
		{
			string name = timers[i]->name;
			timers.erase(timers.begin()+i);
			--i;
		//	Log("Timer triggered");
			TriggerEvent(name);
			continue;
		}
		timers[i]->time -= game->time->deltaTime;
	}
}

void Object::Destroy()
{
	OnDestroy();
	/*
	for(auto& evt : events)
	{
		delete evt;
	}
	*/
}

void Object::OnDestroy()
{
}

void Object::LoadFile(string path)
{
	Log(type::Name(this), " loading file '", path, "'.");
	Properties prop;
	prop.LoadFile(path);
//	string type = type::Unscope(type::Name(this));
//	to_lower(type);
	Log("Checking for first child");
	if(Properties* pc = prop.GetFirstChild())
	{
		Log("Loading first child");
		Load(pc);
	}
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
	string type = type::Unscope(type::Name(this));
	to_lower(type);
	prop->type = type;
}

void Object::Bind()
{
	Script::BaseClass<Object> inst;

	inst.Func("Init", &Object::Init);
	inst.Func("Update", &Object::Update);
	inst.Func("Destroy", &Object::Destroy);
	inst.Func("OnDestroy", &Object::OnDestroy);
	inst.Func("ConnectEvent", &Object::ConnectScriptEvent);
	inst.Func("SetTimer", &Object::SetTimer);
	inst.Func("CancelTimer", &Object::CancelTimer);
	inst.Func("GetTimer", &Object::GetTimer);
	inst.Func("Load", &Object::Load);
	inst.Func("Save", &Object::Save);
	inst.Func("LoadFile", &Object::LoadFile);
	inst.Func("SaveFile", &Object::SaveFile);
}

}
