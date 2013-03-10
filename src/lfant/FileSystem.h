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
#pragma once

#include <lfant/stdafx.h>

// External
#include <boost/filesystem.hpp>

// Internal

#include <lfant/Subsystem.h>
#include <lfant/tinyxml2.h>

using namespace boost::filesystem;

namespace lfant
{
const string GameName = "ShadowFox";

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

// Typedefs for ease-of-use and switchability
typedef tinyxml2::XMLDocument xmldoc;
typedef tinyxml2::XMLElement xmlelem;
typedef tinyxml2::XMLNode xmlnode;
typedef tinyxml2::XMLAttribute xmlattr;

/**
 *
 */
class FileSystem : public Subsystem
{
	friend class Settings;
public:
	FileSystem();
	~FileSystem();

	void Init();

	xmldoc LoadXML(string type, string dir);
	void UnloadXML(xmldoc doc);

//	path GetAssetPath(string name);
	path GetUserPath(string name = "/");
	path GetGamePath(string name = "/");

	vector<path> GetGameFiles(string dir);

protected:
	string ConvertPath(string curr);

public:
	string gameFolder = "../..";
	string userFolder;
};

/** @} */
/** @} */
}
