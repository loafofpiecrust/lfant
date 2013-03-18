
#include <lfant/ScriptSystem.h>

// Internal
#include <lfant/Console.h>
#include <lfant/util/String.h>
#include <lfant/FileSystem.h>
#include <lfant/Time.h>
#include <lfant/Random.h>

// External
#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <boost/chrono.hpp>

namespace lfant
{

Script::Script(string module) :
	builder(new CScriptBuilder)
{
	builder->StartNewModule(game->scriptSystem->scriptEngine, module.c_str());
}

void Script::LoadFile(string path)
{
	Log("Loading script from \'"+game->fileSystem->GetGamePath(path).string()+"\'.");
	auto r = builder->AddSectionFromFile(game->fileSystem->GetGamePath(path).c_str());
	if(r < 0)
	{
		Log("Script::LoadFile: Errors found in "+path+", could not load.");
		return;
	}
}

void Script::Compile()
{
	auto r = builder->BuildModule();
	if( r < 0 )
	{
		Log("Script::Compile: Errors found, could not compile.");
	}
}

void TestFunc()
{
	int i = Random(0, 999);
	int k = Random(9999, 99999999);
	i += k;
	k -= i;
	i -= k;
	k *= i;
	Log("\t\ti = ", i);
//	Log("k = ", k);
}


int myint = 2;

void ScriptSystem::Init()
{
	Log("ScriptSystem::Init: Touch.");
	scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
//	scriptEngine->SetDefaultNamespace("lfant");

	RegisterStdString(scriptEngine);

	RegisterFunction("Log", Log<string>);
	RegisterFunction("Log", Log<string, int32_t>);
	RegisterFunction("TestFunc", TestFunc);

	RegisterVariable("myint", &myint);

	Script test("test");
	test.LoadFile("scripts/test.as");
	test.Compile();

	hclock::duration length;
	hclock::time_point start;

	for(uint i = 0; i < 10; ++i)
	{
		start = hclock::now();
		//test.CallFunction("void main()");
		CallFunction("test", "void main()");
		length = hclock::now() - start;
		Log("\tLength of func from global ", i, ": ", length, ".");
	}

	start = hclock::now();
	TestFunc();
	TestFunc();
	TestFunc();
	length = hclock::now() - start;
	Log("Length of C++ func: ", length, ".");
}

void ScriptSystem::OnDestroy()
{
	scriptEngine->Release();
}

void ScriptSystem::CallFunction(string module, string call)
{
	asIScriptFunction* func = scriptEngine->GetModule(module.c_str())->GetFunctionByDecl(call.c_str());
	if(!func)
	{
		Log("ScriptSystem::CallFunction: Function \'"+call+"\' not found.");
		return;
	}

	asIScriptContext* ctx = scriptEngine->CreateContext();
	ctx->Prepare(func);
	auto r = ctx->Execute();
	if(r != asEXECUTION_FINISHED)
	{
		if(r == asEXECUTION_EXCEPTION)
		{
			Log("ScriptSystem::CallFunction: An exception occurred, \'", ctx->GetExceptionString(), "\'.");
		}
	}
	ctx->Release();
}

template<typename T>
void ScriptSystem::RegisterFunction(string decl, T* func)
{
	size_t start_pos = decl.find("(");
	if(start_pos == string::npos)
	{
		string type = DemangleType(typeid(func).name());
		decl = Replace(type, "(*)", decl);
		decl = ReplaceAll(decl, ",", " a,");
		decl = ReplaceAll(decl, "std::", "");
		size_t start_pos = decl.find("()");
		if(start_pos == string::npos)
		{
			decl = Replace(decl, ")", " a)");
		}
	}
	Log("ScriptSystem::RegisterFunction: Func name: \'"+decl+"\'.");
	scriptEngine->RegisterGlobalFunction(decl.c_str(), asFUNCTION(func), asCALL_CDECL);
}

template<typename T>
void ScriptSystem::RegisterVariable(string name, T* var)
{
	string decl = DemangleType(typeid(T).name())+" "+name;
	Log("ScriptSystem::RegisterVariable: Var name \'"+decl+"\'.");
	scriptEngine->RegisterGlobalProperty(decl.c_str(), var);
}

void Script::Class::Create(string type, bool ref, int size)
{
	Log("Script::Class::Create: Registering object type \'"+type+"\'.");
	this->type = type;
	if(ref)
	{
		game->scriptSystem->scriptEngine->RegisterObjectType(type.c_str(), 0, asOBJ_REF | asOBJ_NOCOUNT);
	}
	else
	{
		game->scriptSystem->scriptEngine->RegisterObjectType(type.c_str(), size, asOBJ_VALUE);
	}
}

template<typename T, typename R>
void Script::Class::Method(string decl, R (T::* func))
{
	game->scriptSystem->scriptEngine->RegisterObjectMethod(type.c_str(), decl.c_str(), asSMethodPtr<sizeof(R (T::*))>::Convert((R (T::*))(func)), asCALL_THISCALL);
}

}
