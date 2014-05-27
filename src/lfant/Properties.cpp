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
	type(type),
	name(name),
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
	ifstream stream(path);
	LoadStream(stream);
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

	for(auto& i : values)
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
	//			GetGame()->Log("ValueProperties::GetChild: Got child namespace '"+child->type+"'.");
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
/*	inst.Func("ValueString", (string (Properties::*)(string))&Properties::Value<string>);
	inst.Func("ValueInt", (int (Properties::*)(string))&Properties::Value<int>);
	inst.Func("ValueFloat", (float (Properties::*)(string))&Properties::Value<float>);
	inst.Func("ValueVec3", (vec3 (Properties::*)(string))&Properties::Value<vec3>);
	inst.Func("ValueVec2", (vec2 (Properties::*)(string))&Properties::Value<vec2>);*/
	inst.Bind("Properties");
}

}
