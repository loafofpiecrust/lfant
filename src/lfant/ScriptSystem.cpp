
#include <lfant/ScriptSystem.h>

// Internal
#include <lfant/Console.h>
#include <lfant/util/String.h>
#include <lfant/FileSystem.h>
#include <lfant/Time.h>
#include <lfant/Random.h>
#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "ScriptComp.h"

// External
#include <sqrat.h>
#include <sqrat/sqratVM.h>
#include <boost/chrono.hpp>

namespace lfant
{

std::deque<Script> Script::scripts;

Script::Script(string p) :
	path(p)
{
	std::cout << "compiling " << path << "\n";
	string err = "";
	inst.CompileFile(path.c_str(), err);
	std::cout << "compile error? " << err << "\n";
}

Script::~Script()
{
	inst.Release();
}

Script* Script::LoadFile(boost::filesystem::path path)
{
	if(boost::filesystem::exists(path))
	{
		for(auto& s : scripts)
		{
			if(s.path == path)
			{
				return &s;
			}
		}
		scripts.push_back(Script(path.string()));
		return &(scripts.back());
	}
	return nullptr;
}

void Script::Run()
{
	string err = "";
	inst.Run(err);
	std::cout << "squirrel error? " << err << "\n";
}

void TestFunc()
{
	int i = random::Range(0, 999);
	int k = random::Range(9999, 99999999);
	i += k;
	k -= i;
	i -= k;
	k *= i;
//	GetGame()->Log("\t\ti = ", i);
//	GetGame()->Log("k = ", k);
}


int myint = 2;

ScriptSystem::ScriptSystem(Game* game) :
	Subsystem(game)
{

}

void printyli(int i)
{
}

void createShiz(ScriptComp* comp)
{
	std::cout << comp->counter << " thingy\n";
}

void printVec3(vec3 v)
{
	std::cout << lexical_cast<string>(v) << "\n";
}

void ScriptSystem::Init()
{
	Subsystem::Init();

	Sqrat::DefaultVM::Set(vm.GetVM());

	Object::ScriptBind();
	Entity::ScriptBind();
	Component::ScriptBind();
	Transform::ScriptBind();
	ScriptComp::ScriptBind();
}

void ScriptSystem::Deinit()
{
	Script::scripts.clear();
}

void ScriptSystem::CallFunction(string module, string call)
{
}

template<typename T>
void ScriptSystem::RegisterFunction(string decl, T* func)
{
	vm.GetRootTable().Func(decl, func);
}

template<typename T>
void ScriptSystem::RegisterVariable(string name, T* var)
{
	vm.GetRootTable().SetInstance(name, var);
}

}
