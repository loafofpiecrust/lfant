
#include <lfant/ScriptSystem.h>

// Internal
#include <lfant/Console.h>
#include <lfant/util/String.h>
#include <lfant/FileSystem.h>
#include <lfant/Time.h>
#include <lfant/Random.h>

// External
#include <sqrat.h>
#include <boost/chrono.hpp>

namespace lfant
{

Script::Script()
{
}

void Script::LoadFile(string path)
{
	inst.CompileFile(game->fileSystem->GetGamePath(path).string().c_str());
}

void Script::Run()
{
	inst.Run();
}

void TestFunc()
{
	int i = random::Range(0, 999);
	int k = random::Range(9999, 99999999);
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
	Subsystem::Init();

	Sqrat::DefaultVM::Set(vm.getVM());
}

void ScriptSystem::OnDestroy()
{
}

void ScriptSystem::CallFunction(string module, string call)
{
}

template<typename T>
void ScriptSystem::RegisterFunction(string decl, T* func)
{
}

template<typename T>
void ScriptSystem::RegisterVariable(string name, T* var)
{
}

}
