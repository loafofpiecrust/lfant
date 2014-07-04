
#include <lfant/ScriptSystem.h>

// Internal
#include <lfant/Console.h>
#include <lfant/util/String.h>
#include <lfant/FileSystem.h>
#include <lfant/Time.h>
#include <lfant/Random.h>
#include <lfant/Thread.h>
#include <lfant/Input.h>
#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "ScriptComp.h"

#include <lfant/Scene.h>
#include <lfant/Time.h>

// External
#include <sqrat.h>
#include <sqrat/sqratVM.h>
#include <sqratimport.h>
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
	ran = true;
}

ScriptSystem::ScriptSystem(Game* game) :
	Subsystem(game)
{

}

void ScriptSystem::Init()
{
	Subsystem::Init();

	Sqrat::DefaultVM::Set(vm.GetVM());
	sqstd_register_systemlib(vm.GetVM());
	sqrat_register_importlib(vm.GetVM());

	/////
	/// @brief 'random' namespace
	/////
/*	Sqrat::Table tb_rand(vm.GetVM());
	vm.GetRootTable().Bind("random", tb_rand);

	tb_rand.Overload<float (*)(float, float)>("Range", &random::Range);
	tb_rand.Overload<int (*)(int, int)>("Range", &random::Range);
	tb_rand.Overload<vec3 (*)(vec3, vec3)>("Range", &random::Range);

	/////
	/// @brief 'thread' namespace
	/////
	Sqrat::Table tb_thread(vm.GetVM());
	vm.GetRootTable().Bind("thread", tb_thread);

	tb_rand.Func("Sleep", &thread::Sleep);*/

	Object::ScriptBind();
	Entity::ScriptBind();
	Component::ScriptBind();
	Transform::ScriptBind();
	ScriptComp::ScriptBind();

	Subsystem::ScriptBind();
	Scene::ScriptBind();
	Time::ScriptBind();
	Input::ScriptBind();
	Game::ScriptBind();

	Properties::ScriptBind();
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
