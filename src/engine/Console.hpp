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
 *	@details
 *
 *	@todo
 *
 */
class Console : public Subsystem
{
public:
	typedef boost::function<void(vector<string>)> CommandFunc;
	typedef void (CommandFuncRaw)(vector<string>);
	typedef boost::function<void()> CommandFuncSimple;
	typedef void (CommandFuncSimpleRaw)();

	struct Command
	{
		string name;
		CommandFunc func;
		string desc;

		Command(string name, CommandFunc func, string desc) :
			name(name), func(func), desc(desc)
		{
		}
	};

	struct CommandSimple
	{
		string name;
		CommandFuncSimple func;
		string desc;

		CommandSimple(string name, CommandFuncSimple func, string desc) :
			name(name), func(func), desc(desc)
		{
		}
	};

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

	void RegisterCommand(string name, CommandFuncRaw func, string desc = "");
	void RegisterCommand(string name, CommandFuncSimpleRaw func, string desc = "");

	void RegisterVar(string name, float val, string desc = "", bool readOnly = false);

	bool CallCommand(string name, vector<string> args);

	Variable* GetVar(string name);
	Command* GetCommand(string name);
	CommandSimple* GetSimpleCommand(string name);

	float GetValue(string name);
	void SetValue(string name, float value);

	static void CmdExit();

protected:

private:
	vector<Command> commands;
	vector<CommandSimple> simpleCommands;
	vector<Variable> variables;

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

} /* namespace sfs */
