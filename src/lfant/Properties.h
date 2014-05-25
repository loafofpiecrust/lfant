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
#include <functional>
#include <boost/any.hpp>
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
public:
	enum class Mode : uint8
	{
		Input,
		Output
	};

	enum class SaveMode : uint8
	{
		Value,
		Pointer
	};

	Properties();
	Properties(string path);
	Properties(Properties* parent, string type, string name);

	virtual ~Properties();

	static void ScriptBind();

	void SaveFile(string path);
	void LoadFile(string path);

	void LoadStream(std::istream& stream);
	void SaveStream(std::ostream& stream);

	string GetString(string name);

	template<typename T>
	void Value(string name, T* value)
	{
		std::cout << "momo1\n";
		if(mode == Mode::Output)
		{
			std::cout << "saving " << name << "\n";
			if(saveMode == SaveMode::Value) values[name] = *value;
			else values[name] = value;
		}
		else
		{
			std::cout << "momo2\n";
			auto iter = values.find(name);
			if(iter != values.end())
			{
				std::cout << "here we are\n";
				boost::any& ref = iter->second;
				std::cout << "type of cast: " << ref.type().name() << "\n";
				if(ref.type() == typeid(std::string))
				{
					*value = lexical_cast<T>(boost::any_cast<std::string>(ref));
				}
				else
				{
					if(saveMode == SaveMode::Value) { *value = boost::any_cast<T>(ref); }
					else { *value = *(boost::any_cast<T*>(ref)); }
				}
			}
		}
	}

	template<typename T>
	void Value(string name, const T& value)
	{
		std::cout << "value value\n";
		if(mode == Mode::Output)
		{
			values[name] = value;
		}
	}

	template<typename T, typename _Key>
	void ValueMap(string type, std::map<_Key, T>& cont, std::function<void(_Key&, T&, Properties*)> each_func)
	{
		if(mode == Mode::Input)
		{
			for(auto& prop : children)
			{
				if(prop->IsType(type))
				{
					_Key k;
					T t;
					each_func(k, t, prop);
					cont.insert(typename std::map<_Key, T>::value_type(std::move(k), std::move(t)));
				}
			}
		}
		else
		{
			for(auto& elem : cont)
			{
				Properties* prop = Child(type, "");
				each_func(const_cast<_Key&>(elem.first), elem.second, prop);
			}
		}
	}

	void Value(string name, Entity*& ent, Scene* scene);

	Properties* GetParent();
	Properties* GetTopParent();
	Properties* Child(string type, string name);
//	virtual Properties* GetChild(uint32 idx) = 0;
	uint32 GetChildCount();
	Properties* GetChild(uint32 idx);

	virtual void Clear();

	void SetType(string type);
	bool IsType(string type);

	Mode GetMode() { return mode; }
	void SetMode(Mode mode);

	SaveMode GetSaveMode() { return saveMode; }
	void SetSaveMode(SaveMode mode);

	string name = "";
	string type = "";
	Mode mode = Mode::Output;
	SaveMode saveMode = SaveMode::Value;
	std::map<string, boost::any> values;
	std::deque<ptr<Properties>> children;


	template<typename T, typename _Con>
	auto ValueArray(string type, _Con& cont, std::function<void(T&, Properties*)> each_func) -> typename std::enable_if<boost::is_same<_Con, std::vector<T>>::value || boost::is_same<_Con, std::deque<T>>::value, void>::type
	{
		if(mode == Mode::Input)
		{
			for(auto& prop : children)
			{
				if(prop->IsType(type))
				{
					typename _Con::value_type v = typename _Con::value_type();
					each_func(v, prop);
					cont.push_back(std::move(v));
				}
			}
		}
		else
		{
			for(auto& elem : cont)
			{
				Properties* prop = Child(type, "");
				each_func(elem, prop);
			}
		}
	}

protected:

	Properties* parent = nullptr;

};

/** @} */
/** @} */
}
