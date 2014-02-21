/*
*	Copyright (C) 2013-2014, by loafofpiecrust
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

using namespace std;

namespace lfant {
namespace type {

string Name()
{
	return "";
}

string Demangle(const string& type)
{
	int result = 0;
	char* c = abi::__cxa_demangle(type.c_str(), 0, 0, &result);
	string final = string(c);
	if(c) delete[] c;
	return final;
}

string Descope(string type, int amount)
{
	if(amount == 0) return type;
	if(amount == -1)
	{
		deque<string> result = Split(type, ":");
		return result[result.size()-1];
	}

	for(uint i = 0; i < type.size(); ++i)
	{
		if(type[i] == ':' && type[i+1] == ':')
		{
			++i;
			type.erase(type.begin(), type.begin()+i+1);
			--amount;
			if(amount == 0) break;
		}
	}
	return type;
}

string Descope(string type, string nspace)
{
	int start = 0;
	for(uint i = 0; i < type.size(); ++i)
	{
		if(type[i] == ':' && type[i+1] == ':')
		{
			if(type.substr(start, i) == nspace)
			{
				type.erase(type.begin(), type.begin()+i+2);
				break;
			}
			start = i+2; ++i;
		}
	}
	return type;

}

}
}
