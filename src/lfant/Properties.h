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
#include <lfant/util/qumap.h>
#include <lfant/ptr.h>
#include <lfant/Range.h>
#include <lfant/util/lexical_cast.h>

// External
#include <map>
#include <iostream>

namespace lfant {

class Entity;

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
//	friend class lfant::editor::gui::Window;
public:

	Properties();
	Properties(string path);
	~Properties();

	void LoadFile(string path);
	void SaveFile(string path);

	void LoadStream(istream& stream);
	void SaveStream(ostream& stream);

	Properties* GetFirstChild();
	Properties* GetChild(string type);
	Properties* GetChildById(string id);

	const deque<ptr<Properties>>& GetChildren();
	deque<Properties*> GetChildren(string type);

	qumap<string, string>& GetValues();

	Properties* AddChild(Properties* prop);
	Properties* AddChild(string type = "", string id = "");
	Properties* AddChild(istream& stream, string type, string id, bool first = true);

	template<typename T>
	void Set(string name, const T& value)
	{
		Set(name, lexical_cast<string>(value));
	}

	void AddValue(string name, string value);
	void SubtractValue(string name, string value);

	template<typename T>
	void Get(string name, T& ref)
	{
		string value = values[name];
		if(value != "")
		{
			ref = lexical_cast<T>(values[name]);
		}
	}

	void Get(string name, Entity*& ref);
	void Set(string name, Entity* const& value);
	void Set(string name, string value);

	template<typename T = string>
	T Get(string name)
	{
		return lexical_cast<T>(values[name]);
	}

	template<typename T>
	void SetEnum(string name, T value)
	{
		enums[name] = lexical_cast<string>((int)value);
	}

	template<typename T>
	void GetEnum(string name, T& ref)
	{
		string e = enums[Get<string>(name)];
		if(e != "")
		{
			ref = (T)lexical_cast<int>(e);
		}
	}

	void GetType(string& ref)
	{
		if(type != "")
		{
			ref = type;
		}
	}

	void GetId(string& ref)
	{
		if(id != "")
		{
			ref = id;
		}
	}

	string type = "";
	string id = "";

	qumap<string, string> values;
	deque<ptr<Properties>> children;

protected:

private:
	Properties(istream& stream, string type, string id = "", Properties* parent = nullptr, bool first = true);
	Properties(ostream& stream, string type, string id = "", Properties* parent = nullptr, bool first = true);

	void SkipSpace(istream& stream);
	string TrimSpace(string str, bool onlyIndent = false);
	string Expand(string value);
	string GetIndent();

	string ParseValue(string val);

	Properties* GetTopParent();

	qumap<string, string> enums;
	Properties* parent = nullptr;
	bool getFirstLine = true;
};

/** @} */
/** @} */
}
