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

#include "Settings.hpp"

// External
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

// Internal
#include "Engine.hpp"
#include "FileManager.hpp"
#include "StringUtil.hpp"
#include "Console.hpp"

using namespace boost::filesystem;

namespace sfs
{

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
	input = game->fileManager->userFolder+"/"+input;
	string cfg = game->fileManager->gameFolder+"/"+defaultFile;
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
			file.open(cfg);

			string line = "";
			vector<string> words;
			vector<string> prefix;
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
				if(words[0] == "=" || words[0] == "#")
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
							name.append(prefix[g]);
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
							continue;
						}
						// beginning namespace
						prefix.push_back(words[i-1]+".");
					}
					else if(words[i] == "}")
					{
						// end namespace
						prefix.pop_back();
					}
				}
			}
			return;
		}
	}
	Log("Settings::LoadSettings: Neither user or default settings could be loaded.");

}

Settings::Var Settings::GetValue(string name)
{
	to_lower(name);
	Var result("", "");
	for(auto& var : variables)
	{
		if(var.name == name)
		{
			result = var;
		}
	}
	if(result.value == "")
	{
		return result;
	}

	vector<string> tokens = Split(result.value, "", "$[]");
	string value = "";
	for(uint k = 0; k < tokens.size(); ++k)
	{
		if(tokens[k] == "$" && tokens.size() >= k+3)
		{
			if(tokens[k+1] == "[" && tokens[k+3] == "]")
			{
				to_lower(tokens[k+2]);
				Var var("", "");
				var = GetValue(tokens[k+2]);
				if(var.s() != "")
				{
					Log("Scoping complete: "+var.s());
					value.append("$["+tokens[k+2]+"]");
					k += 3;
					continue;
				}
				string temp = "";
				vector<string> scope = Split(name, ".", "");
				int idx = scope.size()-1;

				while(var.s() == "")
				{
					for(uint i = 0; i < idx; ++i)
					{
						temp.append(scope[i]+".");
					}
					temp.append(tokens[k+2]);
					var = GetValue(temp);
					Log("Trying to scope: "+temp+", result: "+var.s());
				}
				value.append(var.s());
				k += 3;
			}
		}
		else
		{
			value.append(tokens[k]);
		}
	}
	result.value = value;

	return result;
}

void Settings::SetValue(string name, string value)
{
	vector<string> tokens = Split(value, "", "$()");
	value = "";
	for(uint k = 0; k < tokens.size(); ++k)
	{
		if(tokens[k] == "$" && tokens.size() >= k+3)
		{
			if(tokens[k+1] == "(" && tokens[k+3] == ")")
			{
				to_lower(tokens[k+2]);
				Var var("", "");
				var = GetValue(tokens[k+2]);
				if(var.s() != "")
				{
					Log("Scoping complete: "+var.s());
					value.append(var.s());
					k += 3;
					continue;
				}
				string temp = "";
				vector<string> scope = Split(name, ".", "");
				int idx = 0;

				while(var.s() == "")
				{
					for(uint i = 0; i < scope.size()-1; ++i)
					{
						temp.append(scope[i]+".");
					}
					temp.append(tokens[k+2]);
					var = GetValue(temp);
					Log("Trying to scope: "+temp+", result: "+var.s());
					++idx;
					if(idx >= 10)
					{
						var.Set(tokens[k+2]);
					}
				}
				value.append(var.s());
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
	Log("VALUE: "+value);

	for(auto& var : variables)
	{
		if(var.name == name)
		{
			var.Set(value);
			return;
		}
	}
	Log(name+"="+value);
	variables.push_back(Var(name, value));
}

void Settings::Init()
{
	// Registering Variables
	//SetValue("gameName", "ShadowFox Engine");
	//SetValue("orgName", "ShadowFox Studios");

	LoadSettings();
}

}
