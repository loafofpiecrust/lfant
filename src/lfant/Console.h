/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

#include <lfant/stdafx.h>

// External
#include <boost/any.hpp>
#include <iostream>
#include <fstream>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits.hpp>


// Internal

#include <lfant/Subsystem.h>

namespace lfant
{
/** @addtogroup Game
 *	@{
 */
/** @addtogroup Utilities
 *	@{
 */

/**
 *	Controls a set of variables and commands through text input.
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

		virtual ~Command() {
		}
	};

	struct CommandSimple : public Command
	{
		typedef void (Console::* funcTypeRaw)();
		typedef boost::function<void ()> funcType;
		funcType func;

		CommandSimple(funcType func, string name, string help) :
			Command(name, help), func(func)
		{
		}
	};

	struct CommandDefault : public Command
	{
		typedef void (Console::* funcTypeRaw)(std::deque<string>);
		typedef boost::function<void (std::deque<string>)> funcType;
		funcType func;

		CommandDefault(funcType func, string name, string help) :
			Command(name, help), func(func)
		{
		}
	};

	struct Variable
	{
		string name;
		string help;
		float value;
		bool readOnly;

		Variable(string name, float val, string help = "", bool readOnly = false) :
			name(name), help(help), value(val), readOnly(readOnly)
		{
		}
	};

	Console(Game* game);
	virtual ~Console();

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	virtual void Serialize(Properties* prop);


	void Input(string line);


	void RegisterCommand(CommandDefault::funcTypeRaw func, string name, string help = "");
	void RegisterCommand(CommandSimple::funcTypeRaw func, string name, string help = "");

	void RegisterVar(string name, float val, string help = "", bool readOnly = false);

	bool CallCommand(string name, std::deque<string> args);
	bool CallCommand(string name);

	Command* GetCommand(string name);

	template<typename T>
	T* GetCommand(string name)
	{
		auto cmd = GetCommand(name);
		if(auto t = dynamic_cast<T*>(cmd))
		{
			return t;
		}
		return nullptr;
	}

	float GetValue(string name);
	void SetValue(string name, float value);

	void CmdExit();
	void CmdGetVar(std::deque<string> args);
	void CmdSetVar(std::deque<string> args);
	void CmdHelp(std::deque<string> args);

protected:

private:
	std::deque<ptr<Command>> commands;

};



/// @}
/// @}

} /* namespace lfant */
