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
#include <lfant/util/qumap.h>
#include <lfant/ptr.h>
#include <lfant/Range.h>
#include <lfant/util/lexical_cast.h>

// External
#include <map>
#include <iostream>
//#include <json/value.h>

namespace lfant {

class Entity;
class Scene;

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

/**
 * Usage case:
 *
 * Properties prop; prop.LoadFile("settings.json");
 *
 */
class Properties
{
	class Iterator
	{
	public:
		Iterator(Properties* prop, uint pos) :
			pos(pos), prop(prop)
		{
		}

		Properties* operator*();
		bool operator!=(const Iterator& other) const;
		const Iterator& operator++();

	private:
		uint pos;
		Properties* prop;
	};

//	friend class lfant::editor::gui::Window;
public:

	class Value
	{
	public:
		string str = "";
		const std::type_info* type = &typeid(string);

		Value() {}
		Value(string str) : str(str) {}
		Value(string str, const std::type_info& type) : str(str), type(&type) {}

		operator string()
		{
			return str;
		}

		Value& operator=(const string& str)
		{
			this->str = str;
			return *this;
		}
	};

	Properties();
	Properties(string path);
	Properties(Properties* parent, string type = "", string name = "");
	~Properties();
	
	static void ScriptBind();

	Iterator begin();
	Iterator end();

	void LoadFile(string path);
	void SaveFile(string path);

	void LoadStream(std::istream& stream);
	void SaveStream(std::ostream& stream);

	Properties* GetFirstChild();
	Properties* GetChild(string type, string name = "");
	Properties* GetChild(uint idx);

	void Clear();

	template<typename T>
	void Set(string name, const T& value)
	{
		SetString(name, Value(lexical_cast<string>(value), typeid(T)));
	}

	template<typename T>
	void Get(string name, T& ref)
	{
		string value = GetString(name);
		if(!value.empty())
		{
			ref = lexical_cast<T>(value);
		}
	}

	void Get(string name, Entity*& ref, Scene* scene);
	void Set(string name, Entity* const& value);

	template<typename T = string>
	T Get(string name)
	{
		return lexical_cast<T>(GetString(name));
	}

	string GetName();

//	Properties* AddArray(string name);
	Properties* AddChild(string name = "");

	void SetType(string type);
	void Rename(string name);

	bool IsType(string type);
	bool IsNamed(string name);

	string type = "";
	string name = "";

	std::map<string, Value> values;
	std::deque<ptr<Properties>> children;
protected:

private:
	Properties* GetTopParent();

	string GetString(string name);
	void SetString(string name, Value value);

//	qumap<string, string> enums;
	Properties* parent = nullptr;
//	bool getFirstLine = true;
//	bool isArray = false;

	string Expand(string value);
	static string TrimSpace(const string& str, bool onlyIndent);
	void SkipSpace(std::istream& stream);
	Properties* AddChild(Properties* prop);
	string GetIndent();
};

/** @} */
/** @} */
}
