/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-08-27 by Taylor Snead
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

#include "Console.hpp"

// External
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

// Internal
#include "StringUtil.hpp"

namespace sfs
{

template<>
void Log<const char*>(const char* msg)
{
	game->console->Print(msg);
}

Console::Console()
{
	// TODO Auto-generated constructor stub

}

Console::~Console()
{
	// TODO Auto-generated destructor stub
}

void Console::Init()
{
	ofstream file;
	file.open(logFile.c_str());
	file.clear();
	file.close();

	// Default commands
	RegisterCommand("Exit", CmdExit, "Exit the game");
}

void Console::Input(string line)
{
	vector<string> output = Split(line, " ,", "=();");
	string str = "";
	for(uint i = 0; i < output.size(); ++i)
	{
		str = output[i];
		if(output.size() > 1)
		{
			// If we are calling a function
			if(auto cmd = GetCommand(str))
			{
				vector<string> args;
				if(output[i + 1] == "(")
				{
					for(uint k = i + 2; k < output.size(); ++k)
					{
						if(output[k] == ")")
						{
							i = k;
							break;
						}
						args.push_back(output[k]);
					}
					cmd->func(args);
				}
				else
				{
					cmd->func(args);
				}
			}
			else if(auto cmd = GetSimpleCommand(str))
			{
				if(output[i + 1] == "(")
				{
					for(uint k = i + 2; k < output.size(); ++k)
					{
						if(output[k] == ")")
						{
							i = k;
							break;
						}
					}
				}
				cmd->func();
			}
			// Or if we want help
			else if(str == "help" || str == "h")
			{
				if(auto var = GetVar(output[i + 1]))
				{
					Print(var->name + " (variable): " + var->desc);
				}
				if(auto cmd = GetCommand(output[i + 1]))
				{
					Print(cmd->name + " (command): " + cmd->desc);
				}
				++i;
			}
			// Or if we are setting/getting a variable
			else if(auto var = GetVar(str))
			{
				// if we place an equals and var isn't read-only
				if(output[i + 1] == "=" && output[i + 2] != "")
				{
					if(!var->readOnly)
					{
						var->value = atof(output[i + 2].c_str());
						//var->value = lexical_cast<float>(output[i + 2]);
						//Print(str + " = " + lexical_cast<string>(var->value));
						Print(str + " = " + output[i + 2]);
						i += 2;
					}
					else
					{
						Print("Variable " + str + " is read-only");
						i += 2;
					}
				}
				else
				{
					Print(str + ": " + lexical_cast<string>(var->value));
				}
			}
		}
		else if(auto var = GetVar(str))
		{
			Print(var->name + ": " + lexical_cast<string>(var->value));
		}
		else if(str == ";")
		{
			continue;
		}
		else
		{
			break;
		}
	}
}

void Console::RegisterVar(string name, float value, string desc, bool readOnly)
{
	for(auto & var : variables)
	{
		if(var.name == name)
		{
			var.value = value;
			var.desc = desc;
			var.readOnly = readOnly;
		}
	}
	variables.push_back(Variable(name, value, desc, readOnly));
}

void Console::RegisterCommand(string name, CommandFuncRaw func, string desc)
{
	for(auto & cmd : commands)
	{
		if(cmd.name == name)
		{
			//cmd.func = boost::bind(func);
			cmd.func = func;
			cmd.desc = desc;
			return;
		}
	}
	commands.push_back(Command(name, func, desc));
}

void Console::RegisterCommand(string name, CommandFuncSimpleRaw func, string desc)
{
	for(auto & cmd : simpleCommands)
	{
		if(cmd.name == name)
		{
			cmd.func = func;
			cmd.desc = desc;
			return;
		}
	}
	simpleCommands.push_back(CommandSimple(name, func, desc));
}

bool Console::CallCommand(string name, vector<string> args)
{
	for(auto & cmd : commands)
	{
		if(cmd.name == name)
		{
			cmd.func(args);
			return true;
		}
	}
	return false;
}

Console::Variable* Console::GetVar(string name)
{
	for(auto & var : variables)
	{
		if(var.name == name)
		{
			return &var;
		}
	}
	return nullptr;
}

Console::Command* Console::GetCommand(string name)
{
	for(auto & cmd : commands)
	{
		if(cmd.name == name)
		{
			return &cmd;
		}
	}
	return nullptr;
}

Console::CommandSimple* Console::GetSimpleCommand(string name)
{
	for(auto & cmd : simpleCommands)
	{
		if(cmd.name == name)
		{
			return &cmd;
		}
	}
	return nullptr;
}

float Console::GetValue(string name)
{
	if(auto var = GetVar(name))
	{
		return var->value;
	}
	return 0.0f;
}

void Console::SetValue(string name, float value)
{
	if(auto var = GetVar(name))
	{
		var->value = value;
	}
}

void Console::CmdExit()
{
	game->Exit();
}

}
