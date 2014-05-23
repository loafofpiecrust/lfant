/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Subsystem.h>
//#include <lfant/ScriptComp.h>

// External
#include <sqrat/sqratVM.h>
#include <sqrat/sqratScript.h>
#include <boost/filesystem.hpp>

namespace lfant {

class ScriptComp;

class Script
{
	friend class ScriptSystem;
public:

	template<typename C, typename CC>
	class ClassBase
	{
	public:
		ClassBase()
		{
		}

		template<typename T>
		void Func(string name, T func)
		{
			inst.Func(name.c_str(), func);
		}

		template<typename T>
		void StaticFunc(string name, T func)
		{
			inst.StaticFunc(name.c_str(), func);
		}

		template<typename T>
		void Var(string name, T var)
		{
			inst.Var(name.c_str(), var);
		}

		template<typename T>
		void Prop(string name, T (C::* get)())
		{
			inst.Prop(name.c_str(), get);
		}

		template<typename T>
		void Prop(string name, T (C::* get)() const)
		{
			inst.Prop(name.c_str(), get);
		}

		template<typename T>
		void Prop(string name, T (C::*get)() const, void (C::*set)(T))
		{
			inst.Prop(name.c_str(), get, set);
		}

		template<typename T>
		void StaticVar(string name, T var)
		{
			inst.StaticVar(name.c_str(), var);
		}

		void Bind(string type = "")
		{
			if(type.empty()) type = type::Descope(type::Name<C>(), "lfant");
			Sqrat::RootTable().Bind(type.c_str(), inst);
		}
		
		const CC& GetInst()
		{
			return inst;
		}

	private:
		CC inst;
	};

	template<typename C>
	class BaseClass : public ClassBase<C, Sqrat::Class<C>>
	{
	};

	template<typename C, typename P>
	class Class : public ClassBase<C, Sqrat::DerivedClass<C, P>>
	{
	};

	~Script();

	static Script* LoadFile(boost::filesystem::path path);
	void Run();
	void Call(string func);

	Sqrat::Script& GetInst() { return inst; }

private:
	Script(string p);

	Sqrat::Script inst;
	string path = "";

	static std::deque<Script> scripts;
};

/**
 *
 */
class ScriptSystem : public Subsystem
{
	friend class Script;
public:
	ScriptSystem(Game* game);

	virtual void Init();
	virtual void Deinit();

	template<typename T>
	void RegisterFunction(string decl, T* func);

	template<typename T>
	void RegisterVariable(string name, T* var);


	void CallFunction(string module, string call);

	Sqrat::SqratVM vm;
protected:

private:

};

}
