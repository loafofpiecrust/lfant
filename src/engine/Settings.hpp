/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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
#include "stdafx.hpp"

// External
#include <boost/lexical_cast.hpp>

// Internal
#include "Subsystem.hpp"
#include "StringUtil.hpp"
#include "Range.hpp"
#include "Console.hpp"

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Core
 *	 @{
 */

/**	Manages game settings from a config file for individual users.
 *	@details
 *		Description
 *	@todo
 *		Maybe combine with a ResourceManager that handles file paths, as LoadProfile and SaveProfile;
 *			or keep all that stuff inside this class?
 */
class Settings : public Subsystem
{
	class Var
	{
	public:
		void Set(string value) {
			this->value = value;
		}

		operator string()
		{
			return s();
		}

		operator float()
		{
			return f();
		}

		operator int()
		{
			return i();
		}

		string s()
		{
			Log("Operator string called");
			return value;
		}

		const char* c_str()
		{
			return value.c_str();
		}

		int i()
		{
			return lexical_cast<int>(value);
		}

		float f()
		{
			return lexical_cast<float>(value);
		}

		bool b()
		{
			if(value == "" || value == "false" || value == "0")
			{
				return false;
			}
			else if(value == "true" || value == "1")
			{
				return true;
			}
			else
			{
				return true;
			}
		}

		Range<int> rng()
		{
			vector<string> words = Split(value, " .", "");
			if(words.size() == 1)
			{
				return Range<int>(lexical_cast<int>(words[0]), 0);
			}
			else if(words.size() >= 2)
			{
				return Range<int>(lexical_cast<int>(words[0]), lexical_cast<int>(words[1]));
			}
		}

		vec2 f2()
		{
			vector<string> tokens = Split(value, " x.", "");
			vec2 result(0);
			if(tokens.size() > 0)
			{
				result.x = lexical_cast<float>(tokens[0]);
				if(tokens.size() > 1)
				{
					result.y = lexical_cast<float>(tokens[1]);
				}
			}
			return result;
		}
		vec3 f3()
		{
			vector<string> tokens = Split(value, " x.", "");
			vec3 result(0);
			if(tokens.size() > 0)
			{
				result.x = lexical_cast<float>(tokens[0]);
				if(tokens.size() > 1)
				{
					result.y = lexical_cast<float>(tokens[1]);
					if(tokens.size() > 2)
					{
						result.z = lexical_cast<float>(tokens[2]);
					}
				}
			}
			return result;
		}

		ivec2 i2()
		{
			vector<string> tokens = Split(value, " x.", "");
			ivec2 result(0);
			if(tokens.size() > 0)
			{
				result.x = lexical_cast<int>(tokens[0]);
				if(tokens.size() > 1)
				{
					result.y = lexical_cast<int>(tokens[1]);
				}
			}
			return result;
		}
		ivec3 i3()
		{
			vector<string> tokens = Split(value, " x.", "");
			ivec3 result(0);
			if(tokens.size() > 0)
			{
				result.x = lexical_cast<int>(tokens[0]);
				if(tokens.size() > 1)
				{
					result.y = lexical_cast<int>(tokens[1]);
					if(tokens.size() > 2)
					{
						result.z = lexical_cast<int>(tokens[2]);
					}
				}
			}
			return result;
		}

		operator Range<int>()
		{
			return rng();
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
	Var GetValue(string name);

protected:
	string userFile = "settings.cfg";
	string defaultFile = "settings.cfg";

	vector<Var> variables;

};

/** @} */
/** @} */
}
