
#include "lfant/ScriptComp.h"
#include "FileSystem.h"

namespace lfant {

ScriptComp* ScriptComp::LoadScript(string path)
{
//	ScriptComp* comp;
	Script script;
	script.LoadFile(game->fileSystem->GetGamePath(path).string());
	script.Run();
	ScriptComp* comp = script.GetInst().GetSlot("instance").Cast<ScriptComp*>();
	comp->script = std::move(script);
	return comp;
}

void ScriptComp::ScriptBind()
{
	Script::Class<ScriptComp, Component> inst;
	
	inst.Func("SetObject", &ScriptComp::SetScriptObject);
	
	inst.Bind("Component");
}


void ScriptComp::SetScriptObject(Sqrat::Object obj)
{
	func_init = Sqrat::Function(obj, "Init");
	func_update = Sqrat::Function(obj, "Update");
	func_deinit = Sqrat::Function(obj, "Deinit");
}


void ScriptComp::Init()
{
//	lfant::Component::Init();
	if(!func_init.IsNull())
	{
		func_init();
	}
}

void ScriptComp::Update()
{
//	lfant::Component::Update();
	if(!func_update.IsNull())
	{
		func_update();
	}
}

void ScriptComp::Deinit()
{
//	lfant::Component::Deinit();
	if(!func_deinit.IsNull())
	{
		func_deinit();
	}
	script.GetInst().Release();
}


}