/*
*	Copyright (C) 2013-2014, by loafofpiecrust
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

#include <lfant/Properties.h>

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>
#include <lfant/Range.h>
#include <lfant/util/String.h>
#include <lfant/Entity.h>
#include <lfant/Scene.h>
#include <lfant/Game.h>
#include "ScriptSystem.h"

// External
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/iterator.h>

namespace lfant {

using namespace std;
using namespace boost::algorithm;

Properties::Properties()
{
}

Properties::Properties(string path)
{
	LoadFile(path);
}

Properties::Properties(Properties* parent, string type, string name) :
	name(name),
	type(type),
	parent(parent)
{
	to_lower(type);
	mode = parent->mode;
	saveMode = parent->saveMode;
//	parent->AddChild(this);
}

Properties::~Properties()
{
}

void Properties::LoadFile(string path)
{
	fs::path p(path);
	if(p.extension().string() == ".yaml")
	{
		LoadFileYaml(path);
	}
	else
	{
		ifstream stream(path);
		LoadStream(stream);
	}
}

static string indent = "";

static Properties* LoadYaml(Properties* prop, YAML::Node& node)
{
	std::cout << indent << "yaml type " << node.Type() << " ";
//	if(node.Isstd::cout << " '" << node.Tag() << "'";
	std::cout << "size: " << node.size() << "\n";
	indent.push_back('\t');

	auto n = new Properties(prop, "", "");
	prop->children.push_back(n);

//	std::cout << "has physics? " << node["physics"].IsDefined() << "\n";
	for(YAML::const_iterator it = node.begin(); it != node.end(); ++it)
	{
		if(node.IsMap())
		{
		//	child = it->second;
			YAML::Node child = it->second;
			string key = it->first.as<string>();

			if(child.IsScalar() || child.IsNull())
			{
				n->Value(key, child.as<string>());
				std::cout << indent+"yaml value '" << key << "'\n";
			}
			else
			{
				std::cout << indent+"yaml struct '" << key << "'\n";
				auto added = LoadYaml(n, child);
				added->type = key;
				added->name = key;
			}
		}
		else
		{
			YAML::Node child = *it;

			if(child.IsScalar() || child.IsNull())
			{
			//	n->Value(child.Tag(), child.as<string>());
				std::cout << indent+"yaml value '" << child.Tag() << "'\n";
			}
			else
			{
				std::cout << indent+"yaml struct '" << child.Tag() << "'\n";
				LoadYaml(n, child);
			}
		}
	}
	indent.pop_back();
	return n;
}

void Properties::LoadFileYaml(string path)
{
	YAML::Node node = YAML::LoadFile(path);
	indent = "";
	LoadYaml(this, node);
}

void Properties::SaveFile(string path)
{
	ofstream out {path};
	SaveStream(out);
	out.close();
}

/// @todo Switch to new string instead of erasing?
string TrimSpace(const string& str, bool onlyIndent)
{
	std::size_t firstLetter = str.find_first_not_of(" \t\n");
	std::size_t lastLetter = str.find_last_not_of(" \t\n");

	if(firstLetter == string::npos)
	{
		return "";
	}
	else if(onlyIndent)
	{
		return str.substr(firstLetter);
	}
	else
	{
		return str.substr(firstLetter, lastLetter-firstLetter+1);
	}
}

string Expand(string value)
{
	return value;
}

string GetIndent(Properties* prop)
{
	if(prop->GetParent())
	{
		return GetIndent(prop->GetParent())+"\t";
	}
	else
	{
		return "";
	}
}

bool IsWhitespace(char c)
{
	return c == ' ' || c == '\t';
}

bool IsOperator(char c)
{
	return c == '=' || c == ':';
}

void SkipWhitespace(istream& stream, char& c)
{
	while(IsWhitespace(c))
	{
		c = stream.get();
	}
//	return c;
}

void SkipSpace(istream &stream)
{
	signed char c = stream.get();
	while (isspace(c) && c != EOF)
	{
		c = stream.get();
	}

	// If we are not at the end of the file, then since we found a
	// non-whitespace character, we put the cursor back in front of it.
	if (c != EOF)
	{
		stream.seekg(-1, ios_base::cur);
	}
}

string ReadUntil(istream& stream, char& curr, string toks, bool lined)
{
	string value = "";
//	char curr = stream.get();
//	curr = stream.get();
	while(stream.good())
	{
		for(auto& t : toks)
		{
			if(curr == t) return value;
		}

		if(lined && curr == '\n')
		{
			curr = stream.get();
			SkipWhitespace(stream, curr);
		}

		value.push_back(curr);
		curr = stream.get();
	}
	return value;
}

void Properties::LoadStream(istream& stream)
{
	SetMode(Mode::Output);

	char curr = '\0';
	Properties* obj = this;

	string token = ""; // current token
	string stype = "";
	string sname = "";
	string value = "";
	while(stream.good())
	{
		curr = stream.get();
		if(curr == '/')
		{
			curr = stream.get();
			if(curr == '/')
			{
				while(curr != '\n')
				{
					curr = stream.get();
				}
			}
			else if(curr == '*')
			{
				while(stream.good())
				{
					curr = stream.get();
					if(curr == '*')
					{
						curr = stream.get();
						if(curr == '/')
						{
							curr = stream.get();
							break;
						}
					}
				}
			}
			else
			{
				stream.unget();
			}
		}
		if(IsWhitespace(curr) || curr == '\n' || IsOperator(curr))
		{
			// process token
			if(!token.empty())
			{
				if(token == "{")
				{
					to_lower(stype);
					erase_all(stype, " ");
					erase_all(sname, " ");
					std::cout << GetIndent(this)+"new structure named '"+stype+" "+sname+"' in " << obj << "\n";
					obj = new Properties(obj, stype, sname);
					obj->SetMode(GetMode());
					obj->parent->children.push_back(obj);
					stype.clear();
					sname.clear();
				}
				else if(token == "}")
				{
					if(obj->GetParent()) obj = obj->GetParent();
				}
				else if(stype.empty())
				{
					stype = token;
				}
				else if(sname.empty())
				{
					sname = token;
				}

				// clear token
				token.clear();

			}
			if(IsOperator(curr))
			{
			//	oper = curr;
				value.clear();

				bool lined = false;
				curr = stream.get();
				while(stream.good())
				{
					if(curr == '{')
						lined = true;
					else if(curr == '}')
						lined = false;

					if(!lined && (curr == ';' || curr == '\n'))
					{
						break;
					}

					value.push_back(curr);
					curr = stream.get();
				}
				value = TrimSpace(value, false);
				if(!value.empty()) obj->Value(stype, value);

				stype.clear();
				value.clear();
				sname.clear();
			}
		}
		else
		{
			token.push_back(curr);
		}
	}
	return;
}

void Properties::SaveStream(ostream &stream)
{
//	stream << "// Scene file generated by lfant.\n";
	string ind = GetIndent(this);
	if(ind.size() > 0) ind.erase(ind.begin());

	if(parent)
	{
		stream << ind << type;
		if(!type.empty())
			stream << " ";
		if(!name.empty())
			stream << name;
		stream << "\n"+ind+"{\n";
	}

//	for(auto& i : values)
	{
	//	stream << ind+"\t" << i.first << " = " << i.second << "\n";
	}

	for(uint32 i = 0; i < GetChildCount(); ++i)
	{
		stream << "\n";
		GetChild(i)->SaveStream(stream);
	}

	if(parent)
		stream << ind << "}\n";
}



bool Properties::IsType(string type)
{
	to_lower(type);
	return this->type == type;
}

void Properties::SetMode(Properties::Mode mode)
{
	this->mode = mode;
	for(auto& child : children)
	{
		child->SetMode(mode);
	}
}

void Properties::SetSaveMode(Properties::SaveMode mode)
{
	this->saveMode = mode;
	for(auto& child : children)
	{
		child->SetSaveMode(mode);
	}
}

void Properties::SetType(string type)
{
	to_lower(type);
	this->type = type;
}

void Properties::Clear()
{
	type.clear();
	name.clear();

	values.clear();
	children.clear();
}

Properties* Properties::GetParent()
{
	return parent;
}

Properties* Properties::GetTopParent()
{
	Properties* prop = this;
	while(prop->parent) prop = prop->parent;
	return prop;
}

string Properties::GetString(string name)
{
	/*
	auto iter = values.find(name);
	if(iter == values.end()) return "";

	boost::any& val = iter->second;
	if(val.type() == typeid(int))
	{
		return boost::any_cast<int>()
	}

	return boost::any_cast<string>(values[name]);
	*/
	return "";
}

void Properties::Value(string name, Sqrat::Object obj)
{
	Sqrat::Table tb (obj, obj.GetVM());
	auto cname = name.c_str();


	std::cout << "types table: \n" <<
				 "float: " << OT_FLOAT << "\n" <<
				 "int: " << OT_INTEGER << "\n" <<
				 "string: " << OT_STRING << "\n" <<
				 "class: " << OT_CLASS << "\n" <<
				 "instance: " << OT_INSTANCE << "\n" <<
				 "table: " << OT_TABLE << "\n\n";

	std::cout << "value name: '" << cname << "'.\n";

	if(!obj.IsNull())
	{
		std::cout << "obj type: " << obj.GetType() << "\n";
		if(!tb.IsNull())
		{
			std::cout << "table type: " << tb.GetType() << "\n";
		}
		else
		{
			std::cout << "table is null\n";
		}
	}

	auto slot = tb.GetSlot(cname);
	if(slot.IsNull())
	{
		std::cout << "slot is null\n";
	}
	else
	{
		std::cout << "slot type: " << slot.GetType() << "\n";
	}
	if(mode == Mode::Input)
	{
		string s="";
		switch(slot.GetType())
		{
		case OT_FLOAT:
		{
			float f; Value(name, &f); tb.SetValue(cname, f);
			tb.Bind(cname, slot);
			std::cout << "we set a float val with " << f << " to " << *(tb.GetValue<float>(cname).Get()) << "\n";
			break;
		}
		case OT_INTEGER: int i; Value(name, &i); tb.SetValue(cname, i); break;
		case OT_BOOL: bool b; Value(name, &b); tb.SetValue(cname, b); break;
		case OT_STRING: Value(name, &s); tb.SetValue(cname, s); break;
		default: break;
		}
	}
	else
	{
		switch(slot.GetType())
		{
		case OT_FLOAT: Value(name, tb.GetValue<float>(cname).Get()); break;
		case OT_INTEGER: Value(name, tb.GetValue<int>(cname).Get()); break;
		case OT_BOOL: Value(name, tb.GetValue<bool>(cname).Get()); break;
		case OT_STRING: Value(name, tb.GetValue<string>(cname).Get()); break;
		default: break;
		}
	}
}

void Properties::Value(string name, Entity*& ent, Scene* scene)
{
	if(mode == Mode::Input)
	{
		int id = 0;
		string entname = "";
		Value(name, &id);
		if(id == 0)
		{
			Value(name, &entname);
			if(entname != "")
			{
				/// @todo implement this
				ent = scene->GetRoot()->GetChild(entname, true);
			}
		}
		else
		{
		//	ref = game->scene->GetChildbyId(id);
		}
	}
	else
	{
		Value(name, ent->GetId());
	}
}

uint32 Properties::GetChildCount()
{
	return children.size();
}

Properties* Properties::GetChild(uint32 idx)
{
	if(children.size() <= idx)
	{
		children.push_back(new Properties(this, "", ""));
		return children.back();
	}
	else
	{
		return children[idx];
	}
}

Properties* Properties::Child(string type, string name)
{
	if(mode == Mode::Input)
	{
		to_lower(type);
		for(auto& child : children)
		{
			if(child->IsType(type) && (name.empty() || child->name == name))
			{
				return child;
			}
		}
		return nullptr;
	}
	else
	{
		Properties* prop = new Properties(this, type, name);
		children.push_back(prop);
		return prop;
	}
}

void Properties::ScriptBind()
{
	Script::ClassBase<Properties, Sqrat::Class<Properties, Sqrat::NoCopy<Properties>>> inst;
	/// @todo Implement Properties::Value() for scripts
	inst.Func<void (Properties::*)(string, Sqrat::Object)>("Value", &Properties::Value);
/*	inst.Func("ValueString", (string (Properties::*)(string))&Properties::Value<string>);
	inst.Func("ValueInt", (int (Properties::*)(string))&Properties::Value<int>);
	inst.Func("ValueFloat", (float (Properties::*)(string))&Properties::Value<float>);
	inst.Func("ValueVec3", (vec3 (Properties::*)(string))&Properties::Value<vec3>);
	inst.Func("ValueVec2", (vec2 (Properties::*)(string))&Properties::Value<vec2>);*/
	inst.Bind("Properties");
}

}
