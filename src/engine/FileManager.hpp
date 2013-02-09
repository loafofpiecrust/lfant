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
#pragma once
#include "stdafx.hpp"

// External
#include <boost/filesystem.hpp>

// Internal
#include "Subsystem.hpp"
#include "tinyxml2.hpp"

namespace sfs
{
#if WINDOWS
const string UserPath = getenv("UserProfile");
#elif UNIX
const string UserPath = getenv("HOME");
#endif

const string GameName = "ShadowFox";

/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Subsystems
 *	 @{
 */

// Typedefs for ease-of-use and switchability
typedef tinyxml2::XMLDocument xmldoc;
typedef tinyxml2::XMLElement xmlelem;
typedef tinyxml2::XMLNode xmlnode;
typedef tinyxml2::XMLAttribute xmlattr;

/**	FileManager
 *	@details
 *		Description
 *	@todo
 *		Todo
 */
class FileManager : public Subsystem
{
public:
	FileManager();
	~FileManager();

	void Init();

	xmldoc LoadXML(string type, string dir);
	void UnloadXML(xmldoc doc);

	filesystem::path GetGameFile(string name);
	vector<filesystem::path> GetGameFiles(string dir, string ext = "");
	filesystem::path GetUserFile(string name);

	string ConvertPath(string curr);

protected:

private:
	string gameFolder;
	string userFolder;
};

/** @} */
/** @} */
}
