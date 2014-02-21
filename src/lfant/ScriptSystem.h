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

// External
#include <sqrat.h>

namespace lfant
{

class Script
{
public:

	template<typename C, typename CC>
	class ClassBase
	{
	public:
		ClassBase() {}

		template<typename T>
		void Func(string name, T (C::* func))
		{
			inst.Func(name.c_str(), func);
		}

		template<typename T>
		void StaticFunc(string name, T (C::*func))
		{
			inst.StaticFunc(name.c_str(), func);
		}

		template<typename T>
		void Var(string name, T (C::* var))
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
		void Prop(string name, T (C::*get)(), void (C::*set)(T))
		{
			inst.Prop(name.c_str(), get, set);
		}

		template<typename T>
		void StaticVar(string name, T (C::* var))
		{
			inst.StaticVar(name.c_str(), var);
		}

		void Bind()
		{
			Sqrat::RootTable().Bind(type.c_str(), inst);
		}

	private:
		string type;
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

	Script();

	void LoadFile(string path);
	void Run();

private:
	Sqrat::Script inst;
};

/**
 *
 */
class ScriptSystem : public Subsystem
{
	friend class Script;
public:

	virtual void Init();
	virtual void Deinit();

	template<typename T>
	void RegisterFunction(string decl, T* func);

	template<typename T>
	void RegisterVariable(string name, T* var);


	void CallFunction(string module, string call);

protected:

private:
	Sqrat::SqratVM vm;

};

}
