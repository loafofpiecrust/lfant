/******************************************************************************
*
* LFANT Source
* Copyright (C) 2012-2013 by LazyFox Studios
* Created: Sep 16, 2012 by Taylor Snead
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/

#include <lfant/Subsystem.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Settings.h>
#include <lfant/Scene.h>

// External


namespace lfant {

Subsystem::Subsystem()
{

}

Subsystem::~Subsystem()
{
	// TODO Auto-generated destructor stub
}

void Subsystem::Init()
{
	Object::Init();
	if(type::Name(this) != type::Name<Scene>())
	{
		printf("About to load file for subsystem\n");
		string type = type::Unscope(type::Name(this));
		to_lower(type);
		LoadFile("settings/"+type+".cfg");
	}
}

}
