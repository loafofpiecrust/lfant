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
			while(file.good())
			{
				if(!getline(file, line, ';'))
				{
					break;
				}
				if(line == "")
				{
					continue;
				}
				words = Split(line, " \t\n", "#=");
				Log("Settings::LoadSettings: Split words on a line");
				if(words[0] == "=" || words[0] == "#")
				{
					continue;
				}
				for(uint i = 0; i < words.size(); ++i)
				{
					if(words[i] == "#")
					{
						break;
					}
					else if(words[i] == "=")
					{
						Log("Settings::LoadSettings: Hit an equals");
						if(i == 0)
						{
							break;
						}

						string value = words[i+1];
						if(words.size() > i+1)
						{
							for(uint k = i+1; k < words.size()-(i+1); ++k)
							{
								value.append(" "+words[k]);
							}
						}

						to_lower(words[i-1]);

						SetValue(words[i-1], value);
						break;
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
	for(auto& var : variables)
	{
		if(var.name == name)
		{
			return var;
		}
	}
	return Var("", "");
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
				string temp = GetValue(tokens[k+2]);
				if(temp != "")
				{
					value.append(temp);
				}
				else
				{
					value.append(tokens[k+2]);
				}
			}
		}
		else
		{
			value.append(tokens[k]);
		}
	}

	for(auto& var : variables)
	{
		if(var.name == name)
		{
			var.Set(value);
			return;
		}
	}
	variables.push_back(Var(name, value));
}

void Settings::Init()
{
	// Registering Variables
	SetValue("gameName", "ShadowFox Engine");
	SetValue("orgName", "ShadowFox Studios");

	LoadSettings();
}

}
