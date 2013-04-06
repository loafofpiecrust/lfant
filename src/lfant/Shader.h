/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-29 by Taylor Snead
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
#include <lfant/Object.h>

// External
#include <forward_list>

namespace lfant
{
/** @addtogroup Game
 *	@{
 */
/** @addtogroup Rendering
 *	@{
 */

/**
 *
 */
class Shader : public Object
{
public:

	struct Uniform
	{
		string name = "";
		uint32 id = 0;
	};

	Shader()
	{
	}

	void Load(Properties *prop);
	void Save(Properties* prop);
	void LoadFile(string file = "");

	uint32 GetUniform(string name);
	void AddUniform(string name);

	uint32 GetId();

protected:

	static uint32 Compile(uint32 type, const string& path);

	string vertex = "";
	string fragment = "";
	uint32 id = 0;
	map<string, uint32> uniforms;
};

/// @}
/// @}
}
