/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include "FileManager.hpp"

// External
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>

// Internal

using namespace boost::filesystem;

namespace sfs
{

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::Init()
{
	path p("../system.cfg");
	if(exists(p))
	{
		// Load settings from system.cfg
	}
	else
	{
		// Use default settings
		gameFolder = "../..";
		userFolder = UserPath + "/" + GameName;

		// Save these to system.cfg
	}
}

string FileManager::ConvertPath(string curr)
{
	for(auto & chr : curr)
	{
		// Supported grouping characters here.
		if(chr == '\\' || chr == ',' || chr == ':' || chr == '*' || chr == '/')
		{
			// Convert all grouping chars to forward slashes for directory management.
#		if UNIX
			chr = '/';
#		elif WINDOWS
			chr = '\\';
#		endif
		}
	}
	return curr;
}

path FileManager::GetGameFile(string name)
{
	path result(ConvertPath(gameFolder + "/assets/" + name));

	if(exists(result) && is_regular_file(result))
	{
		return result;
	}
	return "";
}

path FileManager::GetUserFile(string name)
{
	path result(ConvertPath(userFolder + "/" + name));

	if(exists(result) && is_regular_file(result))
	{
		return result;
	}
	return "";
}

vector<path> FileManager::GetGameFiles(string dir, string ext)
{
	vector<path> result;
	if(exists(dir) && is_directory(dir))
	{
		copy(directory_iterator(dir), directory_iterator(), back_inserter(result));
	}
	return result;
}

}
