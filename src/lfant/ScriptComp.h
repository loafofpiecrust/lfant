
#include "lfant/Component.h"
#include "lfant/ScriptSystem.h"


namespace lfant {

class ScriptComp : public Component
{
public:
	
	static ScriptComp* LoadScript(string path);
	
	virtual void Init();
	virtual void Update();
	virtual void Deinit();
	
	static void ScriptBind();
	
protected:
	
private:
	void SetScriptObject(Sqrat::Object obj);
	
	Script script;
	
	// script functions
	Sqrat::Function func_init;
	Sqrat::Function func_update;
	Sqrat::Function func_deinit;
};

}
