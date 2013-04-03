/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-03-16 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
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

// Internal
#include <lfant/ptr.h>
#include <lfant/Range.h>
#include <lfant/util/lexical_cast.h>

// External
#include <map>
#include <iostream>

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

/**
 *
 */
class Properties
{
public:

	Properties();
	Properties(string path);
	~Properties();

	void LoadFile(string path);
	void SaveFile(string path);

	void LoadStream(istream& stream);
	void SaveStream(ostream& stream);

	Properties* GetChild(string type);
	Properties* GetChildById(string id);
	deque<Properties*> GetChildren(string type = "");

	Properties* AddChild(string type = "");

	template<typename T>
	void Set(string name, const T& value)
	{
		name = TrimSpace(name);
		values[name] = lexical_cast<string>(value);
	}

	void AddValue(string name, string value);
	void SubtractValue(string name, string value);

	template<typename T>
	T& Get(string name, T& ref)
	{
		to_lower(name);
		string val = values[name];
		if(val != "")
		{
			ref = lexical_cast<T>(val);
			cout << "\tGetting '" << lexical_cast<string>(ref) << "'.\n\n";
		}
		return ref;
	}

	template<typename T = string>
	T Get(string name)
	{
		to_lower(name);
		cout << "Getting '"+name+"'.\n";
		string val = values[name];
		return lexical_cast<T>(val);
	}

	template<typename T>
	void SetEnum(string name, T value)
	{
		enums[name] = lexical_cast<string>((int)value);
	}

	template<typename T>
	T GetEnum(string name, T& ref)
	{
		string val = enums[values[name]];
		if(val != "")
		{
			ref = (T)lexical_cast<int>(val);
		}
		return ref;
	}

	string type = "";
	string id = "";

protected:

private:
	Properties(istream& stream, string type, string id = "", Properties* parent = nullptr);
	Properties(ostream& stream, string type, string id = "", Properties* parent = nullptr);

	void SkipSpace(istream& stream);
	string TrimSpace(string str, bool onlyIndent = false);
	string Expand(string value);
	string GetIndent();

	map<string, string> values;
	map<string, string> enums;
	deque< ptr<Properties> > children;
	Properties* parent = nullptr;
};

/** @} */
/** @} */
}
