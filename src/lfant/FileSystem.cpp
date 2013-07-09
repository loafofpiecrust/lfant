/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-08-08 by Taylor Snead
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

#include <lfant/FileSystem.h>

// External
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>

// Internal

#include <lfant/Game.h>
#include <lfant/Settings.h>

#include <lfant/Console.h>

using namespace boost::filesystem;

namespace lfant
{

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

#if LINUX
string GetProgramDir()
{
	char tmp[128];
	uint len = readlink("/proc/self/exe", tmp, sizeof(tmp)-1);
	if(len != -1)
	{
		tmp[len] = '\0';
	}
	string result = tmp;
	if(uint pos = result.find("/launcher"))
	{
		result.erase(result.begin()+pos, result.end());
	}
	return result;
}
#elif WINDOWS
string GetProgramDir()
{
	HMODULE hmod = GetModuleHandle(0);
	char file[100];
	GetModuleFileName(hmod, file, 100);

	path p(file);
	p.remove_filename();
	return p.string();
}
#elif MACOSX

#endif

void FileSystem::Init()
{
	// Use default settings
//	gameFolder = "../..";
#if WINDOWS
	const string home = getenv("USERPROFILE");
#elif UNIX
	const string home = getenv("HOME");
#endif
	userFolder = home + "/Documents/My Games/" + game->orgName + "/" + game->gameName;
	gameFolder = GetProgramDir()+"/../..";

	Subsystem::Init();
}

path FileSystem::GetGamePath(string name)
{
	path result(gameFolder + "/assets/" + name);
	if(!exists(result))
	{
		Log("FileSystem::GetGamePath: File not found '"+result.string()+"'.");
	//	game->Exit();
	}
	return result;
}

path FileSystem::GetUserPath(string name)
{
	path result(userFolder + "/" + name);
	if(!exists(result))
	{
		Log("FileSystem::GetUserPath: File not found '"+result.string()+"'.");
	}
	return result;
}

deque<path> FileSystem::GetGameFiles(string dir)
{
	deque<path> result;
	if(exists(dir) && is_directory(dir))
	{
		copy(directory_iterator(dir), directory_iterator(), back_inserter(result));
	}
	return result;
}

}
