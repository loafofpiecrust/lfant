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
		float value;
		string help;
		bool readOnly;

		Variable(string name, float val, string help = "", bool readOnly = false) :
			name(name), value(val), help(help), readOnly(readOnly)
		{
		}
	};

	Console(Game* game);
	virtual ~Console();

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop) const;

	void Input(string line);

	template<typename T = string>
	void Print(const T& msg)
	{
		LinePrint(msg);
		LinePrint("\n");
	}

	template<typename T = string>
	auto LinePrint(T msg)->typename std::enable_if<boost::is_fundamental<T>::value || boost::is_pointer<T>::value || boost::is_same<string, T>::value, void>::type
	{
	//	logFile.open(logName, ios_base::app);
	//	string strMsg = lexical_cast<string>(msg);
		// Print to console window
		std::cout << msg;
		// Append message to file
		logFile << msg;

	//	logFile.close();
	}

	template<typename T = string>
	auto LinePrint(T msg)->typename std::enable_if<!boost::is_fundamental<T>::value && !boost::is_pointer<T>::value && !is_ptr<T>::value && !boost::is_same<string, T>::value, void>::type
	{
		LinePrint(lexical_cast<string>(msg));
	}

	template<typename T>
	auto LinePrint(const T& msg)->typename std::enable_if<is_ptr<T>::value, void>::type
	{
		LinePrint(msg.get());
	}

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

	string logName = "lfant.log";
	std::ofstream logFile;
};



/// @}
/// @}

} /* namespace lfant */
