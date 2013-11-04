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
#include <lfant/util/String.h>

// External

// Internal
#include <lfant/Console.h>


namespace lfant
{

deque<string> Split(string str, string dropDelim, string keepDelim)
{
	using namespace boost;
	deque<string> result;
	if(str == "")
	{
		return result;
	}
	char_separator<char> sep(dropDelim.c_str(), keepDelim.c_str(), drop_empty_tokens);
	tokenizer<char_separator<char>> tok(str, sep);
	for(tokenizer<char_separator<char>>::iterator i = tok.begin(); i != tok.end(); ++i)
	{
		result += *i;
	}
	return result;
}

deque<string> Split(deque<string> str, string dropDelim, string keepDelim)
{
	deque<string> combined;
	for(auto& i : str)
	{
		deque<string> temp = Split(i, dropDelim, keepDelim);
		combined.insert(combined.end(), temp.begin(), temp.end());
	}
	return combined;
}

deque<string> SplitParens(string str, string dropDelim, string keepDelim)
{
	deque<string> result = Split(str, dropDelim, keepDelim);
	for(uint i = 0; i < result.size(); ++i)
	{
		Log("SplitParens: result[", i, "] = ", result[i]);
		if(uint pos = result[i].find("(("))
		{
			result[i].erase(pos+1);
		}
		if(uint pos = result[i].find("))"))
		{
			result[i].erase(pos+1);
		}
		if(result[i][0] == '(')
		{
			result[i].erase(0);
		}
		if(result[i][result[i].size()-1] == ')')
		{
			result[i].erase(result[i].end());
		}
		Log("SplitParens: result[", i, "] = ", result[i]);
	}
	/*
	bool newstr = false;
	int parlev = 0;
	string curr = "";
	for(uint i = 0; i < str.size(); ++i)
	{
		if(parlev <= 0)
		{
			for(uint k = 0; k < dropDelim.size(); ++k)
			{
				if(str[i] == dropDelim[k])
				{
					newstr = true;
					break;
				}
			}
			for(uint k = 0; k < keepDelim.size(); ++k)
			{
				if(str[i] == keepDelim[k])
				{

					curr.push_back(str[i]);
					newstr = true;
					break;
				}
			}
		}

		if(str[i] == '(')
		{
			++parlev;
		}
		else if(str[i] == ')')
		{
			--parlev;
		}

		if(!newstr)
		{
			curr.push_back(str[i]);
		}
		else
		{
			if(curr != "")
			{
				result.push_back(curr);
			}
			newstr = false;
			curr = "";
		}
	}
	*/
}

string Extension(string path)
{
	deque<string> tokens = Split(path, ".", "");
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
