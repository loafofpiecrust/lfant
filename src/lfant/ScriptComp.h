
#pragma once

#include "lfant/Component.h"
#include "lfant/ScriptSystem.h"

#include <sqrat/sqratFunction.h>

namespace lfant {

class ScriptComp : public Component
{
public:

	static ScriptComp* LoadScript(Game* game, string path);

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	static void ScriptBind();

	int counter = 3;

private:
	void SetScriptObject(Sqrat::Object obj);
	
	void ConnectScriptComponent(string name, Sqrat::Object obj);

//	Script* script;

	// script functions
	Sqrat::Function func_init;
	Sqrat::Function func_update;
	Sqrat::Function func_deinit;

};

}
