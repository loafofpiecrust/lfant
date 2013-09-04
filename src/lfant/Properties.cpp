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

#include <lfant/Properties.h>

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>
#include <lfant/Range.h>
#include <lfant/util/String.h>
#include <lfant/Entity.h>
#include <lfant/Scene.h>
#include <lfant/Game.h>

// External

namespace lfant
{

Properties::Properties()
{
}

Properties::Properties(string path)
{
	LoadFile(path);
}

Properties::Properties(istream &stream, string type, string id, Properties *parent, bool first) :
	type(type), id(id), parent(parent), getFirstLine(first)
{
	LoadStream(stream);
}

Properties::~Properties()
{
}

void Properties::Get(string name, Entity*& ref)
{
	int id = 0;
	string entname = "";
	Get(name, id);
	if(id == 0)
	{
		Get(name, entname);
		if(entname != "")
		{
			ref = game->scene->GetEntity(entname);
		}
	}
	else
	{
		ref = game->scene->GetEntityById(id);
	}
}

void Properties::Set(string name, Entity* const& value)
{
	Set(name, value->GetId());
}

void Properties::LoadFile(string path)
{
	ifstream stream(game->fileSystem->GetGamePath(path).string());
	LoadStream(stream);
}

void Properties::LoadStream(istream& stream)
{
	string line = "";
	string extline = "";
	uint pos;
	uint pos2;
	deque<string> stack;
	deque<string> toks;
	string name = "";
	string value = "";
	char c;
	bool firstLine = true;

//	Set("type", type);
//	Set("name", id);
//	if(getFirstLine && stream.good())
//		getline(stream, line);

	while(stream.good())
	{
	//	if((firstLine && getFirstLine) || !firstLine)
		{
			getline(stream, line);
		//	firstLine = false;
		}

		pos = line.find("\\");
		while( pos != -1)
		{
			getline(stream, extline);
			line.erase(pos);
			line.append(extline);
			pos = line.find("\\");
		}

		line = TrimSpace(line, true);

		pos = line.find("//");
		if(pos != -1)
		{
			Log("Comment found.");
			line.erase(line.begin()+pos, line.end());
		}

		if(line.find("/*") != -1)
		{
			while(true)
			{
				if(line.find("*/") != string::npos)
				{
					break;
				}
				getline(stream, line);
			}
		}

		if(line == "")
		{
			continue;
		}

		if(line[0] == '=')
		{
			Log("Incorrect '='.");
		}

		if(line[0] == '#')
		{
			// Preprocessor magictastics!
			Log("Doing some preprocessor stuff...");
		//	Preprocess(string(&line[1]));
			continue;
		}
		/*
		if(line[0] == '}')
		{
			continue;
		}
		*/
		// Add to value
		pos = line.find("+=");
		if(pos != -1)
		{
			for(int i = 0; i < pos; ++i)
			{
				name.push_back(line[i]);
			}
			for(int i = pos+2; i < line.size(); ++i)
			{
				value.push_back(line[i]);
			}
		//	name = ;
		//	to_lower(name);
			value = TrimSpace(value, true);
			AddValue(TrimSpace(name), value);
			name = "";
			value = "";
			continue;
		}

		// Subtract from value
		pos = line.find("-=");
		if(pos != -1)
		{
			for(int i = 0; i < pos; ++i)
			{
				name.push_back(line[i]);
			}
			for(int i = pos+2; i < line.size(); ++i)
			{
				value.push_back(line[i]);
			}
		//	to_lower(name);
			value = TrimSpace(value, true);
			SubtractValue(name, value);
			name = "";
			value = "";
			continue;
		}

		// Set value
		pos = line.find("=");
		pos2 = line.find("{");
		if(pos != -1)
		{
			if(pos2 != -1)
			{
				// We have an array on our hands
				for(int i = 0; i < pos; ++i)
				{
					name.push_back(line[i]);
				}
				for(int i = pos2; i < line.size(); ++i)
				{
					value.push_back(line[i]);
				}

				uint pos3 = value.find("}");
				if(pos3 == -1)
				{
					while((c = stream.get()) != '}')
					{
						if(c == '\n' || c == '\t')
							continue;
						value.push_back(c);
					}
				}

			//	to_lower(name);
				Set(name, TrimSpace(value, true));
				name = "";
				value = "";

				continue;
			}

			for(int i = 0; i < pos; ++i)
			{
				name.push_back(line[i]);
			}
			for(int i = pos+1; i < line.size(); ++i)
			{
				value.push_back(line[i]);
			}
		//	to_lower(name);
			Log("\tSetting, '"+TrimSpace(name)+"' = '"+TrimSpace(value, true)+"'.");
			Set(name, TrimSpace(value, true));
			name = "";
			value = "";
			continue;
		}

		// Function call
		// @todo Needed?

		// Namespacing
		toks = Split(line, " ", "{}");
		if(toks[0] != "{" && toks[0] != "}")
		{
			Log("Setting name.");
			name = toks[0];
			to_lower(name);
		}
		if(toks.size() > 1 && toks[1] != "{")
		{
			Log("Setting value");
			value = toks[1];
			if(toks.size() > 2 && toks[2] == "{")
			{
				Log("Namespace brace same line");
				if(toks.size() > 3 && toks[toks.size()-1] == "}")
				{
					Log("One line class");
					if(toks.size() > 4)
					{
						AddChild(stream, name, value, false);
					}
					else
					{
						Properties* c = new Properties;
						c->type = name;
						c->id = value;
						c->parent = this;
						children.push_back(c);
					}

				//	children.push_back(new Properties(stream, name, value, this, false));
					continue;
				}
				else
				{
					children.push_back(new Properties(stream, name, value, this));
				//	continue;
				}
			}
		}
	//	if(toks.size() > 1 && toks[toks.size()-1] == "{")
		{
			Log("Namespace begins same line");
		//	children.push_back(new Properties(stream, name, value, this));
		}
		/*
		for(int i = 1; i < toks.size(); ++i)
		{
			if(toks[i] == "{")
			{
				// Begin namespace
				// @todo Fix this for the ending '}'
			//	to_lower(name);
			//	children.push_back(new Properties(stream, name, value, this));

				if(toks[i+1] == "}")
				{
					// Empty namespace, next line
					break;
					continue;
				}
			}
			else if(toks[i] == "}")
			{
				// End namespace?
				// @todo maybe do namespace queue?
				break;
				continue;
			}
		}
		*/
		// Brace on new line?
		if(line[0] == '{' || stream.get() == '{')
		{
			to_lower(name);
			children.push_back(new Properties(stream, name, value, this));
		}
		else
		{
			stream.seekg(-1, ios_base::cur);
			
		}

		if((line[0] == '}' || stream.get() == '}'))
		{
			Log("First char is finishing brace");
			// Ends a namespace
			return;
		}
		else
		{
			stream.seekg(-1, ios_base::cur);
		}
	}
}

void Properties::SaveStream(ostream &stream)
{
//	stream << "// Scene file generated by lfant.\n";
	stream << GetIndent() << type << " " << id << "\n";

	stream << GetIndent() << "{\n";

	for(auto& i : values)
	{
		stream << GetIndent()+"\t" << i.first << " = " << i.second << "\n";
	}

	for(auto& child : children)
	{
		child->SaveStream(stream);
	}

	stream << GetIndent() << "}\n";
}

void Properties::AddValue(string name, string value)
{
	name = TrimSpace(name);
	string valname = Get<string>(name);
//	value = Expand(value);
	if(valname == "")
	{
		Get(name, valname);
	}
	Set(name, valname+value);
//	values[name].append(value);
	Log(name, " += ", value);
}

void Properties::SubtractValue(string name, string value)
{
	name = TrimSpace(name);
	string val = Get<string>(name);
//	value = Expand(value);
//	values[name] = Replace(values[name], value, "");
	Set(name, Replace(val, value, ""));
	Log(name, " -= ", value);
}


void Properties::SaveFile(string path)
{
	path = game->fileSystem->GetGamePath(path).string();
	Log("Saving properties to '"+path+"'.");
	ofstream stream(path);
	SaveStream(stream);
}

Properties* Properties::GetFirstChild()
{
	if(children.size() > 0)
	{
		return children[0];
	}
	return nullptr;
}

Properties* Properties::GetChild(string type)
{
	to_lower(type);
	for(auto& child : children)
	{
		if(child->type == type)
		{
			Log("Properties::GetChild: Got child namespace '"+child->type+"'.");
			return child;
		}
	}
	return nullptr;
}

Properties* Properties::GetChildById(string id)
{
	for(auto& child : children)
	{
		if(child->id == id)
		{
			return child;
		}
	}
	return nullptr;
}

deque<Properties*> Properties::GetChildren(string type)
{
	to_lower(type);
	deque<Properties*> props;
	for(auto& child : children)
	{
		if(child->type == type)
		{
			props.push_back(child);
		}
	}
	return props;
}

qumap<string, string>& Properties::GetValues()
{
	return values;
}

Properties* Properties::AddChild(string type, string id)
{
	Properties* result = new Properties;
	result->parent = this;
	result->type = type;
	result->id = id;
	children.push_back(result);
	return result;
}

Properties* Properties::AddChild(istream& stream, string type, string id, bool first)
{
	Properties* c = new Properties();
	c->type = type;
	c->id = id;
	c->parent = this;
	c->getFirstLine = first;
	c->LoadStream(stream);
	children.push_back(c);
	return c;
}

void Properties::SkipSpace(istream &stream)
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

string Properties::TrimSpace(string str, bool onlyIndent)
{
	deque<string> toks = Split(str, "", " \t");
	string result = "";
	for(int i = 0; i < toks.size(); ++i)
	{
		if(toks[i] != " " && toks[i] != "\t")
		{
			result.append(toks[i]);
			if(onlyIndent)
			{
				for(uint k = i+1; k < toks.size(); ++k)
				{
					result.append(toks[k]);
				}
				return result;
			}
		}
	}

	return result;
}

string Properties::Expand(string value)
{
	/*
	deque<string> tokens = Split(value, "", "\"' \t");
	string result = "";
	bool quoted = false;
	for(uint i = 0; i < tokens.size(); ++i)
	{
		if(tokens[i] == "\"" || tokens[i] == "'")
		{
			quoted = !quoted;
		}
		else
		{
			if(quoted)
			{
				result.append(tokens[i]);
			}
			else
			{
				if(tokens[i] == " " || tokens[i] == "\t")
				{
					continue;
				}
				result.append(tokens[i]);
			}
		}
	}
	return result;
	*/
	return value;
}

string Properties::GetIndent()
{
	if(parent)
	{
		return parent->GetIndent()+"\t";
	}
	return "";
}


}
