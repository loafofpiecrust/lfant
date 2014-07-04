
#include "lfant/ScriptComp.h"
#include "FileSystem.h"
#include "Time.h"
#include "Thread.h"
#include <sqrat/sqratScript.h>
#include <sqrat/sqratVM.h>
#include <sqrat/sqratTable.h>
#include <sqrat/sqratFunction.h>

namespace lfant {

ScriptComp::~ScriptComp()
{
}

ScriptComp* ScriptComp::LoadScript(Game* game, string path)
{
//	ScriptComp* comp;
	Script* script = Script::LoadFile(path);
	script->Run();

	fs::path p {path};
	string className = p.replace_extension().filename().string();

	Sqrat::Script scr;
	scr.CompileString("instance <- "+className+"();");
	scr.Run();

	Sqrat::Object obj = game->scriptSystem->vm.GetRootTable().GetSlot("instance");
//	Sqrat::Object obj = scr.GetSlot("instance");
	if(obj.IsNull())
	{
		std::cout << "shit we farwqed up\n";
		return nullptr;
	}

	ScriptComp* pcomp = obj.Cast<ScriptComp*>();
	game->scriptSystem->vm.GetRootTable().SetInstance<ScriptComp*>("instance", nullptr);
//	Sqrat::Object cls = game->scriptSystem->vm.GetRootTable().GetSlot("Elevator");

	if(!pcomp)
	{
		std::cout << "we got a null\n";
	}
//	pcomp->SetScriptObject(obj);
	std::cout << "counter: " << pcomp->counter << "\n";
//	std::cout << "script obj1: " << obj.Cast<Object*>()  << "\n";
//	thread::Sleep(2000);
	return pcomp;
}

void ScriptComp::ScriptBind()
{
	Script::Class<ScriptComp, Component> inst;

	inst.Func("SetObject", &ScriptComp::SetScriptObject);
	inst.Func("ConnectComponent", &ScriptComp::ConnectScriptComponent);
	inst.Var("counter", &ScriptComp::counter);

	inst.Bind("Component");

}


void ScriptComp::SetScriptObject(Sqrat::Object obj)
{
	std::cout << "setting up script obj\n";
//	std::cout << "script obj2: " << obj.Cast<Object*>() << "\n";
	func_init = Sqrat::Function(obj, "Init");
	func_update = Sqrat::Function(obj, "Update");
	func_deinit = Sqrat::Function(obj, "Deinit");
	func_serial = Sqrat::Function(obj, "Serialize");
	object = obj;

	if(func_update.IsNull())
	{
		std::cout << "we be fucked\n";
	}
}


void ScriptComp::Init()
{
//	lfant::Component::Init();
	if(!func_init.IsNull())
	{
		std::cout << "about to init\n";
		func_init.Execute();
	}
}

void ScriptComp::Update()
{
//	lfant::Component::Update();
//	std::cout << "calling the shit";
	if(!func_update.IsNull())
	{
		func_update.Execute(GetGame()->time->deltaTime);
	}
}

void ScriptComp::Deinit()
{
//	lfant::Component::Deinit();
	if(!func_deinit.IsNull())
	{
		func_deinit.Execute();
	}
}

void ScriptComp::DestroyFromScript(ScriptComp* comp)
{
	if(!comp->object.IsNull())
	{
		comp->GetGame()->Log("Releasing object from script.");
		comp->object.Release();
	}
}

void ScriptComp::Serialize(Properties* prop)
{
	Component::Serialize(prop);
	if(!func_serial.IsNull())
	{
		func_serial.Execute(prop);
	}
/*	auto tb = (Sqrat::Table)object;
	if(prop->mode == Properties::Mode::Input)
	{
		for(auto& val : prop->values)
		{
			Sqrat::Object child = tb.GetSlot(val.first.c_str());
			if(child.IsNull()) continue;
			string avalue = boost::any_cast<string>(val.second);
			switch(child.GetType())
			{
			case OT_FLOAT: tb.SetValue(val.first.c_str(), lexical_cast<float>(avalue)); break;
			case OT_INTEGER: tb.SetValue(val.first.c_str(), lexical_cast<int>(avalue)); break;
			case OT_BOOL: tb.SetValue(val.first.c_str(), lexical_cast<bool>(avalue)); break;
			case OT_STRING: tb.SetValue(val.first.c_str(), avalue); break;
			default: break;
			}
			GetGame()->Log("\n\nWe saved some stuff\n\n");
		}
	}*/
}

void ScriptComp::ConnectScriptComponent(string name, Sqrat::Table obj, string item)
{
	Component* comp = GetOwner()->GetComponent(name);
	if(comp)
	{
		obj.SetInstance(item.c_str(), comp);
	}
	ConnectEvent(owner, "SetComponent"+name, obj.GetSlot(item.c_str()));
//	Component* val = (owner->GetComponent(name));
/*	if(!val)
	{
		val = dynamic_cast<T*>(Component::NewFromString(type));
		owner->AddComponent(val);
	}*/
}


}
