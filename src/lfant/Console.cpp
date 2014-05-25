/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-08-27 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Console.h>

// Internal
#include <lfant/util/String.h>
#include <lfant/FileSystem.h>
#include <lfant/Game.h>

// External
#include <lfant/util/lexical_cast.h>
#include <boost/format.hpp>

using namespace std;

namespace lfant {

Console::Console(Game* game) :
	Subsystem(game)
{
}

Console::~Console()
{
}

void Console::Init()
{
	Subsystem::Init();

	logFile.open(game->GetProgramDir()+"/"+logName);
	logFile.clear();

	// Default commands
	RegisterCommand(&Console::CmdExit, "quit", "Quit the game");
	RegisterCommand(&Console::CmdGetVar, "get");
	RegisterCommand(&Console::CmdSetVar, "set");
	RegisterCommand(&Console::CmdHelp, "help");
//	RegisterCommand(&Console::CmdSpawn, "spawn");
//	RegisterCommand(&Console::CmdAddComp, "addcomp");
	//	RegisterCommand(&Console::CmdLoadFile, "loadfile");
}

void Console::Update()
{
}

void Console::Deinit()
{
	logFile.close();
}

void Console::Serialize(Properties *prop)
{
	Subsystem::Serialize(prop);

	prop->Value("logFile", &logName);
}

void Console::CmdGetVar(deque<string> args)
{
//	GetGame()->Log("get: Value of '" + args[0] + "':" + game->settings->GetValue(args[0]));
}

void Console::CmdSetVar(deque<string> args)
{
	if(args.size() < 2)
	{
		GetGame()->Log("set: No value given.");
		return;
	}
//	game->settings->SetValue(args[0], args[1]);
//	GetGame()->Log("set: Changed '" + args[0] + "' to " + game->settings->GetValue(args[0]));
}

void Console::CmdHelp(deque<string> args)
{
	if(Command* cmd = GetCommand(args[0]))
	{
		GetGame()->Log("help: "+cmd->help);
		return;
	}
	/*
	string var = game->settings->GetHelp(args[0]);
	if(var != "")
	{
		GetGame()->Log("help: "+var);
		return;
	}
	*/
	GetGame()->Log("help: none found.");
}

void Console::Input(string line)
{
	deque<string> output = Split(line, " ", "");
	string str = "";
	for(uint i = 0; i < output.size(); ++i)
	{
		str = output[i];
		if(output.size() > 0)
		{
			// If we are calling a function
			if(auto cmd = GetCommand<CommandDefault>(str))
			{
				if(output.size() <= 1)
				{
					break;
				}
				deque<string> args;
				for(uint k = i + 1; k < output.size(); ++k)
				{
					args.push_back(output[k]);
				}
				cmd->func(args);
			}
			else if(auto cmd = GetCommand<CommandSimple>(str))
			{
				cmd->func();
			}
			else
			{
				Print("Console::Input: Unknown command \'"+output[i]+"\'");
			}
			/*
			   // Or if we want help
			   else if(str == "help" || str == "h")
			   {
					if(auto var = GetVar(output[i + 1]))
					{
							Print(var->name + " (variable): " + var->help);
					}
					if(auto cmd = GetCommand(output[i + 1]))
					{
							Print(cmd->name + " (command): " + cmd->help);
					}
			   ++i;
			   }
			 */
			/*
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
			 */
		}
		/*
		   else if(auto var = GetVar(str))
		   {
				Print(var->name + ": " + lexical_cast<string>(var->value));
		   }
		   else if(str == ";")
		   {
				continue;
		   }
		 */
		else
		{
			break;
		}
	}
}

void Console::RegisterCommand(CommandDefault::funcTypeRaw func, string name, string help)
{
	for(auto& cmd : commands)
	{
		CommandDefault* d = dynamic_cast<CommandDefault*>(cmd.get());
		if(d && cmd->name == name)
		{
			d->func = boost::bind(func, this, _1);
			d->help = help;
			return;
		}
	}
	commands.push_back(new CommandDefault(boost::bind(func, this, _1), name, help));
}

void Console::RegisterCommand(CommandSimple::funcTypeRaw func, string name, string help)
{
	for(auto& cmd : commands)
	{
		CommandSimple* s = dynamic_cast<CommandSimple*>(cmd.get());
		if(s && cmd->name == name)
		{
			s->func = boost::bind(func, this);
			s->help = help;
			return;
		}
	}
	commands.push_back(new CommandSimple(boost::bind(func, this), name, help));
}

bool Console::CallCommand(string name, deque<string> args)
{
	for(auto& cmd : commands)
	{
		auto d = dynamic_cast<CommandDefault*>(cmd.get());
		if(d && cmd->name == name)
		{
			d->func(args);
			return true;
		}
	}
	return false;
}

bool Console::CallCommand(string name)
{
	for(auto& cmd : commands)
	{
		auto s = dynamic_cast<CommandSimple*>(cmd.get());
		if(s && cmd->name == name)
		{
			s->func();
			return true;
		}
	}
	return false;
}

Console::Command* Console::GetCommand(string name)
{
	for(auto& cmd : commands)
	{
		if(cmd->name == name)
		{
			return cmd;
		}
	}
	return nullptr;
}

void Console::CmdExit()
{
	game->Exit();
}

}
