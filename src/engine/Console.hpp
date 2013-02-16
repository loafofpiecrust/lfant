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
#pragma once
#include "stdafx.hpp"

// External
#include <boost/any.hpp>
#include <iostream>
#include <fstream>

// Internal
#include "Subsystem.hpp"
#include "Engine.hpp"

namespace sfs
{
/** @addtogroup Engine
 *	@{
 */
/** @addtogroup Utilities
 *	@{
 */

/**
 *	Controls a set of variables and commands through text input.
 *	@details
 *		Supports getting and setting variables, calling callback commands,
 *		and recreation of a few commands/abilities of a standard linux
 *		bash terminal.
 *	@todo
 *		Add a few basic bash commands for filesystem management.
 */
class Console : public Subsystem
{
public:
	struct Command
	{
		string name;
		string help;

		Command(string name, string help) :
			name(name), help(help)
		{
		}
	};

	struct CommandSimple : public Command
	{
		typedef boost::function<void()> funcType;
		funcType func;

		CommandSimple(funcType func, string name, string help) :
			Command(name, help), func(func)
		{
		}
	};

	struct CommandDefault : public Command
	{
		typedef boost::function<void(vector<string>)> funcType;
		funcType func;

		CommandDefault(funcType func, string name, string help) :
			Command(name, help), func(func)
		{
		}
	}

	struct Variable
	{
		string name;
		float value;
		string desc;
		bool readOnly;

		Variable(string name, float val, string desc = "", bool readOnly = false) :
			name(name), value(val), desc(desc), readOnly(readOnly)
		{
		}
	};

	Console();
	virtual ~Console();

	virtual void Init();
	virtual void Update()
	{
	}
	virtual void OnDestroy()
	{
	}

	void Input(string line);

	template<typename T = string>
	void Print(T msg)
	{
		ofstream file;
		// Print to console window
		cout << msg << "\n";
		// Load log file
		file.open(logFile.c_str(), ios::app);
		// Append message to file
		file << msg << "\n";
		file.close();
	}

	template<typename T = string>
	void LinePrint(T msg)
	{
		ofstream file;
		// Print to console window
		cout << msg;
		// Load log file
		file.open(logFile.c_str(), ios::app);
		// Append message to file
		file << msg;
		file.close();
	}

	void RegisterCommand(CommandDefault::funcType func, string name, string help = "");
	void RegisterCommand(CommandSimple::funcType func, string name, string help = "");

	void RegisterVar(string name, float val, string help = "", bool readOnly = false);

	bool CallCommand(string name, vector<string> args);
	bool CallCommand(string name);

	Variable* GetVar(string name);
	Command* GetCommand(string name);

	float GetValue(string name);
	void SetValue(string name, float value);

	static void CmdExit();

protected:

private:
	vector<Command*> commands;
	vector<Variable*> variables;

	string logFile = "game.log";
};

template<typename T = const char*>
void Log(T msg)
{
	game->console->Print(msg);
}

template<typename T, typename P, typename ... A>
void Log(T msg, P msg2, A ... args)
{
	game->console->LinePrint(msg);
	Log(msg2, args...);
}

/// @}
/// @}

} /* namespace sfs */
