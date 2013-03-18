/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-03-16 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
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

// Internal
#include <lfant/ptr.h>
#include <lfant/Range.h>

// External

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

/**
 *
 */
class Properties
{
public:

	Properties();
	Properties(string path);
	~Properties();

	void LoadFile(string path);
	void SaveFile(string path);

	Properties* GetChild(string type);
	Properties* GetChildById(string id);
	vector<Properties*> GetChildren(string type = "");

	template<typename T>
	T Get(string name, T defval = T())
	{
		string val = values[name];
		if(val == "")
		{
			return defval;
		}
		return lexical_cast<T>(val);
	}

	string type;
	string id;

protected:

private:
	Properties(ifstream& stream, string type, string id = "");

	void LoadStream(ifstream& stream);

	void SkipWhiteSpace(ifstream& stream);
	char* TrimWhiteSpace(char* str);

	map<string, string> values;
	vector< ptr<Properties> > children;
};

/** @} */
/** @} */
}
