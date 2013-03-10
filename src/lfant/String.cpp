/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-27 by Taylor Snead
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
#include <lfant/String.h>

// External

// Internal


namespace lfant
{

vector<string> Split(string str, string dropDelim, string keepDelim)
{
	using namespace boost;
	vector<string> result;
	if(str == "")
	{
	return result;
	}
	char_separator<char> sep(dropDelim.c_str(), keepDelim.c_str(), drop_empty_tokens);
	tokenizer<char_separator<char> > tok(str, sep);
	for(tokenizer<char_separator<char> >::iterator i = tok.begin(); i != tok.end(); ++i)
	{
		result += *i;
	}
	return result;
}

vector<string> Split(vector<string> str, string dropDelim, string keepDelim)
{
	vector<string> combined;
	for(auto& i : str)
	{
		vector<string> temp = Split(i, dropDelim, keepDelim);
		combined.insert(combined.end(), temp.begin(), temp.end());
	}
	return combined;
}

string Extension(string path)
{
	vector<string> tokens = Split(path, ".", "");
	if(tokens.size() > 1)
	{
		to_lower(tokens[tokens.size()-1]);
		return tokens[tokens.size()-1];
	}
	else
	{
		return "";
	}
}

string Replace(string str, string find, string replace)
{
	size_t start_pos = str.find(find);
	if(start_pos == string::npos)
	{
		return str;
	}

	str.replace(start_pos, find.length(), replace);
	return str;
}

string ReplaceAll(string str, string find, string replace)
{
	if(find.empty())
	{
		return str;
	}

	size_t start_pos = 0;
	while((start_pos = str.find(find, start_pos)) != string::npos)
	{
		str.replace(start_pos, find.length(), replace);
		start_pos += replace.length();         // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return str;
}

}
