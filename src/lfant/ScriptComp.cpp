
#include "lfant/ScriptComp.h"
#include "FileSystem.h"
#include "Time.h"
#include "Thread.h"
#include <sqrat/sqratScript.h>
#include <sqrat/sqratVM.h>
#include <sqrat/sqratTable.h>
#include <sqrat/sqratFunction.h>

namespace lfant {

ScriptComp* ScriptComp::LoadScript(Game* game, string path)
{
//	ScriptComp* comp;
	Script* script = Script::LoadFile(path);
	script->Run();

	/*
	Sqrat::Function func = game->scriptSystem->vm.GetRootTable().GetFunction("Create");
	if(func.IsNull())
	{
		std::cout << "shit we farwqed up\n";
		return nullptr;
	}

	auto comp = func.Evaluate<ScriptComp>();
	std::cout << "ScriptComp*: " << comp.Get() << "\n";

	auto obj = func.Evaluate<Sqrat::Object>();
	std::cout << "Object: " << obj << "\n";
	*/
	Sqrat::Object obj = game->scriptSystem->vm.GetRootTable().GetSlot("instance");
	if(obj.IsNull())
	{
		std::cout << "shit we farwqed up\n";
		return nullptr;
	}

	ScriptComp* pcomp = obj.Cast<ScriptComp*>();
	std::cout << "pointer: " << pcomp << "\n";

//	std::cout << game->scriptSystem->instComp << "\n";

//	ScriptComp* comp = obj.Cast<ScriptComp*>();
//	std::cout << comp << "\n";

	Sqrat::Object cls = game->scriptSystem->vm.GetRootTable().GetSlot("Elevator");

	std::cout << "pointer: " << cls.GetSize() << "\n";

	pcomp->SetScriptObject(obj);
//	comp->SetScriptObject(obj);
//	thread::Sleep(5000);
//	return comp;
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
	func_init = Sqrat::Function(obj, "Init");
	func_update = Sqrat::Function(obj, "Update");
	func_deinit = Sqrat::Function(obj, "Deinit");

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
		func_init.Execute();
	}
}

void ScriptComp::Update()
{
//	lfant::Component::Update();
	if(!func_update.IsNull())
	{
	//	std::cout << "calling the shit";
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

void ScriptComp::ConnectScriptComponent(string name, Sqrat::Object obj)
{
	ConnectEvent(owner, "SetComponent"+name, obj);
//	Component* val = (owner->GetComponent(name));
/*	if(!val)
	{
		val = dynamic_cast<T*>(Component::NewFromString(type));
		owner->AddComponent(val);
	}*/
}


}
