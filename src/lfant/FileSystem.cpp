/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-08-08 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/FileSystem.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Console.h>

// External
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>

#if WINDOWS
#include <direct.h>
#endif

using namespace std;

namespace lfant {


FileSystem::FileSystem(Game* game) :
	Subsystem(game)
{
}

FileSystem::~FileSystem()
{
}


void FileSystem::Init()
{
	// Use default settings
//	gameFolder = "../..";

	Subsystem::Init();
}


}
