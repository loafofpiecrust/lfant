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

// External
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>

// Internal
#include <lfant/Game.h>
#include <lfant/Console.h>

using namespace boost::filesystem;
using namespace std;

namespace lfant {

#if LINUX || ANDROID
string GetProgramDirRaw()
{
	char tmp[128];
	uint len = readlink("/proc/self/exe", tmp, sizeof(tmp)-1);
	if(len != -1)
	{
		tmp[len] = '\0';
	}
	path result {tmp};

	return result.remove_filename().string();
}
#elif WINDOWS
string GetProgramDirRaw()
{
	HMODULE hmod = GetModuleHandle(0);
	char file[128];
	GetModuleFileName(hmod, file, 128);

	path p(file);
	p.remove_filename();
	return p.string();
}
#elif MACOSX

#endif

FileSystem::FileSystem() :
	programFolder(GetProgramDirRaw()),
	gameFolder(programFolder+"/../../..")
{
}

FileSystem::~FileSystem()
{
}

string FileSystem::GetProgramDir()
{
	return programFolder;
}

void FileSystem::Init()
{
	// Use default settings
//	gameFolder = "../..";
#if WINDOWS
	const string home = getenv("USERPROFILE");
#elif UNIX
	const string home = getenv("HOME");
#elif ANDROID
	const string home = getenv("HOME");
#endif
	userFolder = home + "/Documents/My Games/" + game->orgName + "/" + game->gameName;

	Subsystem::Init();
}

path FileSystem::GetGamePath(string name) const
{
	path result(gameFolder + "/assets/" + name);
	if(!exists(result))
	{
		Log("FileSystem::GetGamePath: File not found '"+result.string()+"'.");
	//	game->Exit();
	}
	return result;
}

path FileSystem::GetUserPath(string name) const
{
	path result(userFolder + "/" + name);
	if(!exists(result))
	{
		Log("FileSystem::GetUserPath: File not found '"+result.string()+"'.");
	}
	return result;
}

deque<path> FileSystem::GetGameFiles(string dir) const
{
	deque<path> result;
	if(exists(dir) && is_directory(dir))
	{
		copy(directory_iterator(dir), directory_iterator(), back_inserter(result));
	}
	return result;
}

}
