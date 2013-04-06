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

Properties::Properties(istream &stream, string type, string id, Properties *parent) :
	type(type), id(id), parent(parent)
{
	LoadStream(stream);
}

Properties::~Properties()
{
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
	int pos;
	int pos2;
	deque<string> stack;
	deque<string> toks;
	string name = "";
	string value = "";
	char c;

//	Set("type", type);
//	Set("name", id);

	while(stream.good())
	{
		getline(stream, line);

		pos = line.find("\\");
		while( pos != string::npos)
		{
			getline(stream, extline);
			line.erase(pos);
			line.append(extline);
			pos = line.find("\\");
		}

		line = TrimSpace(line, true);

		pos = line.find("//");
		if(pos != string::npos)
		{
			Log("Comment found.");
			line.erase(line.begin()+pos, line.end());
		}

		if(line.find("/*") != string::npos)
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

		// Add to value
		pos = line.find("+=");
		if(pos != string::npos)
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
			to_lower(name);
			value = TrimSpace(value, true);
			AddValue(TrimSpace(name), value);
			name = "";
			value = "";
			continue;
		}

		// Subtract from value
		pos = line.find("-=");
		if(pos != string::npos)
		{
			for(int i = 0; i < pos; ++i)
			{
				name.push_back(line[i]);
			}
			for(int i = pos+2; i < line.size(); ++i)
			{
				value.push_back(line[i]);
			}
			to_lower(name);
			value = TrimSpace(value, true);
			SubtractValue(name, value);
			name = "";
			value = "";
			continue;
		}

		// Set value
		pos = line.find("=");
		pos2 = line.find("{");
		if(pos != string::npos)
		{
			if(pos2 != string::npos)
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

				int pos3 = value.find("}");
				if(pos3 == string::npos)
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
			Log("\tSetting, '"+TrimSpace(name)+" = "+TrimSpace(value, true)+"'.");
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
			name = toks[0];
		}
		if(toks.size() > 1 && toks[1] != "{")
		{
			value = toks[1];
		}

		for(int i = 1; i < toks.size(); ++i)
		{
			if(toks[i] == "{")
			{
				// Begin namespace
				// @todo Fix this for the ending '}'
				to_lower(name);
				children.push_back(ptr<Properties>(new Properties(stream, name, value, this)));

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

		// Brace on new line?
		if(line[0] == '{' || stream.get() == '{')
		{
			to_lower(name);
			children.push_back(ptr<Properties>(new Properties(stream, name, value, this)));
		}
		else
		{
			stream.seekg(-1, ios_base::cur);
		}

		if((line[0] == '}' || stream.get() == '}'))
		{
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

	map<string,string>::iterator iter;
	for(iter = values.begin(); iter != values.end(); ++iter)
	{
		stream << GetIndent()+"\t" << iter->first << " = " << iter->second << "\n";
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
//	value = Expand(value);
	Log("Adding to '"+values[name]+"', named '"+name+"'.");
	if(values[name] == "")
	{
		Get(name, values[name]);
	}
	values[name].append(value);
	Log(name, " += ", value);
}

void Properties::SubtractValue(string name, string value)
{
	name = TrimSpace(name);
//	value = Expand(value);
	values[name] = Replace(values[name], value, "");
	Log(name, " -= ", value);
}


void Properties::SaveFile(string path)
{
	path = game->fileSystem->GetGamePath(path).string();
	Log("Saving properties to '"+path+"'.");
	ofstream stream(path);
	SaveStream(stream);
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

map<string, string>& Properties::GetValues()
{
	return values;
}

Properties* Properties::AddChild(string type)
{
	Properties* result = new Properties;
	result->parent = this;
	children.push_back(ptr<Properties>(result));
	return result;
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
