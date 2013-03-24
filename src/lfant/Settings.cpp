/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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

#include <lfant/Settings.h>


// Internal
#include <lfant/Game.h>
#include <lfant/FileSystem.h>
#include <lfant/util/String.h>
#include <lfant/Console.h>

// External
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

using namespace boost::filesystem;

namespace lfant
{

template<>
Range<int> Settings::Var::Get< Range<int> >()
{
	vector<string> words = Split(value, " .:-", "");
	if(words.size() == 1)
	{
		return Range<int>(lexical_cast<int>(words[0]), 0);
	}
	else if(words.size() >= 2)
	{
		return Range<int>(lexical_cast<int>(words[0]), lexical_cast<int>(words[1]));
	}
}

template<>
vec2 Settings::Var::Get<vec2>()
{
	vector<string> tokens = Split(value, " x.:", "");
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

template<>
vec3 Settings::Var::Get<vec3>()
{
	vector<string> tokens = Split(value, " x.:", "");
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

template<>
ivec2 Settings::Var::Get<ivec2>()
{
	vector<string> tokens = Split(value, " x.:", "");
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

template<>
ivec3 Settings::Var::Get<ivec3>()
{
	vector<string> tokens = Split(value, " x.:", "");
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

Settings::Settings()
{
}

Settings::~Settings()
{
}

void Settings::LoadSettings(string input)
{
	if(input == "")
	{
		input = userFile;
	}
	input = game->fileSystem->userFolder+"/settings/"+input;
	string cfg = game->fileSystem->gameFolder+"/settings/"+defaultFile;
	if(exists(cfg.c_str()))
	{
		if(exists(input.c_str()))
		{
			Log("Settings::LoadSettings: User settings found.");
			cfg = input;
		}
		else
		{
			Log("Settings::LoadSettings: User settings not found. Using default settings.");
		}

		if(is_regular_file(cfg.c_str()))
		{
			Log("Settings::LoadSettings: Loading settings from "+cfg);

			ifstream file;
			string line = "";
			vector<string> words;
			vector<string> prefix;
			string lastWord;

			file.open(cfg);
			while(file.good())
			{
				if(!getline(file, line))
				{
					break;
				}
				if(line == "")
				{
					continue;
				}
				words = Split(line, " \t", "#=:;{}");
				if(words[0] == "=" || words[0] == "#" || words[0] == ":" || words.size() < 1)
				{
					continue;
				}

				bool equals = false;
				equals = false;
				string name = "";
				string value = "";
				for(uint i = 0; i < words.size(); ++i)
				{
					if(words[i] == "#")
					{
						lastWord = "";
						break;
					}
					else if(words[i] == ";")
					{
						equals = false;
						continue;
					}
					else if(equals)
					{
						value.append(" "+words[i]);
						if(i+1 >= words.size())
						{
							SetValue(name, value);
						}
					}
					else if(words[i] == "=" || words[i] == ":")
					{
						if(i == 0 || words.size() < i+1)
						{
							break;
						}

						for(uint g = 0; g < prefix.size(); ++g)
						{
							name.append(prefix[g]+".");
						}
						name.append(words[i-1]);
						to_lower(name);
						value = words[i+1];

						if(words.size() > i+2)
						{
							equals = true;
						}
						else
						{
							SetValue(name, value);
						}
						++i;
					}
					else if(words[i] == "{")
					{
						if(i == 0 || words.size() < i+1)
						{
							if(lastWord != "")
							{
								prefix.push_back(lastWord);
							}
							continue;
						}
						// beginning namespace
						prefix.push_back(words[i-1]);
					}
					else if(words[i] == "}")
					{
						// end namespace
						prefix.pop_back();
					}
				}

				lastWord = words[words.size()-1];
			}
			return;
		}
	}
	Log("Settings::LoadSettings: Neither user or default settings could be loaded.");

}

void Settings::SetValue(string name, string value, string help)
{
	value = GetRef(name, value, "$()");

	for(auto& var : variables)
	{
		if(var.name == name)
		{
			var.Set(value);
			var.help = help;
			return;
		}
	}
	Log(name+"="+value);
	variables.push_back(Var(name, value, help));
}

string Settings::GetHelp(string name)
{
	for(auto& var : variables)
	{
		if(var.name == name)
		{
			return var.help;
		}
	}
}

string Settings::GetRef(string name, string value, string ids)
{
	vector<string> tokens = Split(value, "", ids);
	string f = "", s = "", t = "";
	f.push_back(ids[0]);
	s.push_back(ids[1]);
	t.push_back(ids[2]);
	value = "";
	for(uint k = 0; k < tokens.size(); ++k)
	{
		if(tokens[k] == f && tokens.size() >= k+3)
		{
			if(tokens[k+1] == s && tokens[k+3] == t)
			{
				to_lower(tokens[k+2]);
				auto var = GetValue(tokens[k+2]);
				if(var != "")
				{
					value.append(f+s+tokens[k+2]+t);
					k += 3;
					continue;
				}
				string temp = "";
				vector<string> scope = Split(name, ".", "");
				int idx = scope.size()-1;

				while(var == "")
				{
					for(uint i = 0; i < idx; ++i)
					{
						temp.append(scope[i]+".");
					}
					temp.append(tokens[k+2]);
					var = GetValue(temp);
				}
				value.append(var);
				k += 3;
			}
			else
			{
				value.append(tokens[k]);
			}
		}
		else
		{
			value.append(tokens[k]);
		}
	}
	return value;
}

void Settings::Init()
{
	// Registering Variables
	//SetValue("gameName", "LFANT");
	//SetValue("orgName", "LazyFox Studios");

	LoadSettings();
}

void Settings::Load(Properties* prop)
{
	prop = prop->GetChild("settings");
	prop->Get("orgName", orgName);
	prop->Get("gameName", gameName);
}

}
