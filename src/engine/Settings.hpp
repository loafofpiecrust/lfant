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

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Subsystems
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

		template<typename T>
		T cast()
		{
			return lexical_cast<T>(value);
		}

		operator string()
		{
			return value;
		}

		operator int()
		{
			return lexical_cast<int>(value);
		}

		operator float()
		{
			return lexical_cast<float>(value);
		}

		operator Range<int>()
		{
			vector<string> words = Split(value, " .", "");
			if(words.size() < 2)
			{
				return Range<int>(0,0);
			}
			return Range<int>(lexical_cast<int>(words[0]), lexical_cast<int>(words[1]));
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

public:
	string userFile = "settings.cfg";
	string defaultFile = "settings.cfg";

	// Game Publishing Info
	string gameName = "ShadowFox Engine";
	string organization = "ShadowFox Studios";

	// Configuration
	int glMajor = 3;
	int glMinor = 3;

	vector<Var> variables;

};

/** @} */
/** @} */
}
