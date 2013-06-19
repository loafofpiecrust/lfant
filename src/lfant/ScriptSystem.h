/******************************************************************************
*
* LFANT Source
* Copyright (C) 2012-2013 by LazyFox Studios
* Created: 2012-09-02 by Taylor Snead
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Subsystem.h>

// External

class asIScriptEngine;
class CScriptBuilder;

namespace lfant
{

class Script
{
public:

	class Class
	{
public:
		Class() {
		}

		template<typename T>
		void Create(string type, bool ref)
		{
			Create(type, ref, sizeof(T));
		}

		template<typename T, typename R>
		void Method(string decl, R (T::* func));

		void Variable();
		void Operator();


private:
		void Create(string type, bool ref, int size);

		string type;
	};

	Script(string module);

	void LoadFile(string path);
	void Compile();

private:
	CScriptBuilder* builder;
};

/**
 *
 */
class ScriptSystem : public Subsystem
{
	friend class Script;
public:

	virtual void Init();

	void OnDestroy();

	template<typename T>
	void RegisterFunction(string decl, T* func);

	template<typename T>
	void RegisterVariable(string name, T* var);


	void CallFunction(string module, string call);

protected:

private:

	asIScriptEngine* scriptEngine;

};

}
