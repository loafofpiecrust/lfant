/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-08-05 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#pragma once

#include <lfant/stdafx.h>

// External
#include <boost/lexical_cast.h>

// Internal

#include <lfant/Subsystem.h>
#include <lfant/String.h>
#include <lfant/Range.h>

#include <lfant/Console.h>

namespace lfant
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

/**
 *	Manages game settings from a config file for individual users.
 *	@todo
 *		Possibly combine with FileSystem
 */
class Settings : public Subsystem
{
	class Var
	{
	public:
		void Set(string value) {
			this->value = value;
		}

		template<typename T>
		T Get()
		{
			return lexical_cast<T>(value);
		}

		Var(string name, string value) : name(name)
		{
			to_lower(this->name);
			Set(value);
		}

		string name = "";
		string value = "";
	};

public:
	Settings();
	~Settings();

	virtual void Init();

	virtual void LoadSettings(string input = "");

	void SetValue(string name, string value);

	template<typename T = string>
	T GetValue(string name)
	{
		to_lower(name);
		Var result("", "");
		for(auto& var : variables)
		{
			if(var.name == name)
			{
				result = var;
			}
		}
		if(result.value == "")
		{
			return result.Get<T>();
		}

		result.Set(GetRef(name, result.Get<string>(), "$[]"));

		return result.Get<T>();
	}

protected:

	string GetRef(string name, string value, string ids = "$()");

	string userFile = "settings.cfg";
	string defaultFile = "settings.cfg";

	vector<Var> variables;

};

/** @} */
/** @} */
}
