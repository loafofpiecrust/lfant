
#pragma once

#include "lfant/Component.h"
#include "lfant/ScriptSystem.h"

#include <sqrat/sqratFunction.h>

namespace lfant {

class ScriptComp : public Component
{
public:
	virtual ~ScriptComp();
	static ScriptComp* LoadScript(Game* game, string path);

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	virtual void Serialize(Properties *prop);

	static void ScriptBind();

	static void DestroyFromScript(ScriptComp* comp);

	int counter = 3;

private:
	void SetScriptObject(Sqrat::Object obj);

	void ConnectScriptComponent(string name, Sqrat::Table obj, string item);

//	Script* script;

	// script functions
	Sqrat::Function func_init;
	Sqrat::Function func_update;
	Sqrat::Function func_deinit;
	Sqrat::Function func_serial;
	Sqrat::Object object;

};

}
