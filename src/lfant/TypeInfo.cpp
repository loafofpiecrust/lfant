/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-28 by Taylor Snead
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

#include <lfant/TypeInfo.h>

// Internal

// External
#include <lfant/util/lexical_cast.h>

namespace lfant {

namespace type {

string Type()
{
	return "";
}

string DemangleType(string type)
{
	return string(abi::__cxa_demangle(type.c_str(), 0, 0, (int*)0));
}

string RemoveScoping(string type)
{
	deque<string> toks = Split(type, ":");
	return toks[toks.size()-1];
}
}

}

