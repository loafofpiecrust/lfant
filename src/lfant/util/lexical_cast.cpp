
#include <lfant/util/lexical_cast.h>

// Internal
#include <lfant/util/String.h>
#include <lfant/Range.h>
#include <lfant/Console.h>

// External
#include <boost/lexical_cast.hpp>

namespace lfant
{

/*
 *	Defaults
 */

template<typename Target, typename Source>
Target lexical_cast(const Source& src)
{
	return boost::lexical_cast<Target>(src);
}

template<>
string lexical_cast<string, string>(const string& src)
{
	return src;
}

template<>
string lexical_cast<string, const char*>(const char* const& src)
{
	return string(src);
}


template<>
string lexical_cast<string, void*>(void* const& src)
{
//	return to_string(src);
}

/*
 *	Primitives from string
 */

template<>
byte lexical_cast<byte, string>(const string& src)
{
	return (byte)strtoul(src.c_str(), 0, 0);
}

template<>
short lexical_cast<short, string>(const string& src)
{
	return atoi(src.c_str());
}

template<>
unsigned short lexical_cast<unsigned short, string>(const string& src)
{
	return (unsigned short)strtoul(src.c_str(), 0, 0);
}

template<>
int lexical_cast<int, string>(const string& src)
{
	return atoi(src.c_str());
}

template<>
unsigned int lexical_cast<unsigned int, string>(const string& src)
{
	return (unsigned int)strtoul(src.c_str(), 0, 0);
}

template<>
long lexical_cast<long, string>(const string& src)
{
	return atol(src.c_str());
}

template<>
unsigned long lexical_cast<unsigned long, string>(const string& src)
{
	return strtoul(src.c_str(), 0, 0);
}

template<>
long long lexical_cast<long long, string>(const string& src)
{
	return atoll(src.c_str());
}

template<>
unsigned long long lexical_cast<unsigned long long, string>(const string& src)
{
	return strtoull(src.c_str(), 0, 0);
}

template<>
float lexical_cast<float, string>(const string& src)
{
	return strtof(src.c_str(), 0);
}

template<>
double lexical_cast<double, string>(const string& src)
{
	return atof(src.c_str());
}

template<>
bool lexical_cast<bool, string>(const string& src)
{
	if(src == "true")
	{
		return true;
	}
	else if(src == "false")
	{
		return false;
	}

	int i = lexical_cast<int>(src);
	if(i > 0)
	{
		return true;
	}
	return false;
}

/*
 *	Primitives _to_ string
 */

template<>
string lexical_cast<string, int>(const int& src)
{
	return to_string(src);
}

template<>
string lexical_cast<string, unsigned char>(const unsigned char& src)
{
	string final = "";
	final.push_back(src);
	return final;
}

template<>
string lexical_cast<string, unsigned short>(const unsigned short& src)
{
	return to_string(src);
}

template<>
string lexical_cast<string, unsigned int>(const unsigned int& src)
{
	return to_string(src);
}

template<>
string lexical_cast<string, unsigned long>(const unsigned long& src)
{
	return to_string(src);
}

template<>
string lexical_cast<string, unsigned long long>(const unsigned long long& src)
{
	return to_string(src);
}
template<>
string lexical_cast<string, bool>(const bool& src)
{
	if(src)
	{
		return "true";
	}
	return "false";
}

template<>
string lexical_cast<string, float>(const float& src)
{
	return boost::lexical_cast<string>(src);
}

template<>
string lexical_cast<string, ivec2>(const ivec2& src)
{
	return "("+lexical_cast<string>(src.x)+","+lexical_cast<string>(src.y)+")";
}

template<>
string lexical_cast<string, vec3>(const vec3& src)
{
	return "("+lexical_cast<string>(src.x)+","+lexical_cast<string>(src.y)+","+
			lexical_cast<string>(src.z)+")";
}

template<>
string lexical_cast<string, vec4>(const vec4& src)
{
	return "("+lexical_cast<string>(src.x)+","+lexical_cast<string>(src.y)+","+
			lexical_cast<string>(src.z)+","+lexical_cast<string>(src.w)+")";
}

template<>
string lexical_cast<string, rgba>(const rgba& src)
{
	return "("+lexical_cast<string>(src.r)+","+lexical_cast<string>(src.g)+","+
			lexical_cast<string>(src.b)+","+lexical_cast<string>(src.a)+")";
}

template<>
string lexical_cast<string, mat4>(const mat4& src)
{
	string result = "\n[\n";
	for(uint i=0; i<4; ++i)
	{
		//if(i != 0)
		{
			result.append(" ");
		}
		for(uint k=0; k<4; ++k)
		{
			result.append(lexical_cast<string>(src[k][i]));
			if(k != 3)
			{
				result.append(", ");
			}
		}
		if(i != 3)
		{
			result.append("\n");
		}
	}
	result.append("\n]");
	return result;
}

template<>
string lexical_cast<string, Range<int> >(const Range<int>& src)
{
	return lexical_cast<string>(src.min)+"-"+lexical_cast<string>(src.max);
}

template<>
string lexical_cast<string, Range<float> >(const Range<float>& src)
{
	return lexical_cast<string>(src.min)+"-"+lexical_cast<string>(src.max);
}

template<>
string lexical_cast<string, Range<Range<float> > >(const Range<Range<float> >& src)
{
	return "("+lexical_cast<string>(src.start)+")-("+lexical_cast<string>(src.end)+")";
}

template<>
string lexical_cast<string, Range<rgba> >(const Range<rgba>& src)
{
	return "("+lexical_cast<string>(src.min)+")-("+lexical_cast<string>(src.max)+")";
}


/*
 *	Primitive to primitive
 */
template<>
unsigned short lexical_cast<unsigned short, char>(const char& src)
{
	return src;
}

/*
 *	GLM Types from string
 */

template<>
ivec2 lexical_cast<ivec2, string>(const string& val)
{
	ivec2 result(0);
	deque<string> str = lfant::Split(val, " x:,()");
	result.x = lexical_cast<int>(str[0]);
	if(str.size() > 1)
	{
		result.y = lexical_cast<int>(str[1]);
	}
	return result;
}

template<>
vec2 lexical_cast<vec2, string>(const string& val)
{
	vec2 result(0);
	deque<string> str = lfant::Split(val, " x:,()");
	result.x = lexical_cast<float>(str[0]);
	if(str.size() > 1)
	{
		result.y = lexical_cast<float>(str[1]);
	}
	return result;
}

template<>
ivec3 lexical_cast<ivec3, string>(const string& val)
{
	ivec3 result(0);
	deque<string> str = lfant::Split(val, " x:,()");
	result.x = lexical_cast<int>(str[0]);
	if(str.size() > 1)
	{
		result.y = lexical_cast<int>(str[1]);
		if(str.size() > 2)
		{
			result.z = lexical_cast<int>(str[2]);
		}
	}
	return result;
}

template<>
vec3 lexical_cast<vec3, string>(const string& val)
{
	vec3 result(0);
	deque<string> str = lfant::Split(val, " x:,()");
	Log("Lexcasting vec3, size: ", str.size());
	result.x = lexical_cast<float>(str[0]);
	if(str.size() > 1)
	{
		result.y = lexical_cast<float>(str[1]);
		if(str.size() > 2)
		{
			result.z = lexical_cast<float>(str[2]);
		}
	}
	Log("Lexcast vec3(", result.x, ",", result.y, ",", result.z, ")");
	return result;
}

template<>
rgba lexical_cast<rgba, string>(const string& src)
{
	rgba result(0);
	deque<string> str = lfant::Split(src, " x.,()");
	result.r = lexical_cast<byte>(str[0]);
	if(str.size() > 1)
	{
		result.g = lexical_cast<byte>(str[1]);
		if(str.size() > 2)
		{
			result.b = lexical_cast<byte>(str[2]);
			if(str.size() > 3)
			{
				result.a = lexical_cast<byte>(str[3]);
			}
		}
	}
	return result;
}


/*
 *	Lfant Types from string
 */

template<>
Range<int> lexical_cast<Range<int>, string>(const string& src)
{
	Range<int> result(0);
	deque<string> str = lfant::Split(src, " .-()");
	result.min = lexical_cast<int>(str[0]);
	if(str.size() > 1)
	{
		result.max = lexical_cast<int>(str[1]);
	}
	return result;
}

template<>
Range<float> lexical_cast<Range<float>, string>(const string& src)
{
	Range<float> result(0);
	deque<string> str = lfant::Split(src, " .,-()");
	result.min = lexical_cast<float>(str[0]);
	if(str.size() > 1)
	{
		result.max = lexical_cast<float>(str[1]);
	}
	return result;
}

/*
template<>
Range<rgba> lexical_cast<Range<rgba>, string>(const string &src)
{
	Range<rgba> result(rgba(0));
	deque<string> str = lfant::SplitParens(src, " .,-");
	result.min = lexical_cast<rgba>(str[0]);
	if(str.size() > 1)
	{
		result.max = lexical_cast<rgba>(str[1]);
	}
	return result;
}

template<>
Range<Range<float>> lexical_cast<Range<Range<float>>, string>(const string& src)
{
	Range<Range<float>> result(0);
	deque<string> str = lfant::Split(src, " .,-()");
	result.min = lexical_cast<Range<float>>(str[0]+"-"+str[1]);
	if(str.size() > 2)
	{
		result.max = lexical_cast<Range<float>>(str[2]+"-"+str[3]);
	}
	return result;
}

template<>
Range<Range<rgba>> lexical_cast<Range<Range<rgba>>, string>(const string& src)
{
	Range<Range<rgba>> result;
	deque<string> str = lfant::SplitParens(src, " .,-");
	result.min = lexical_cast<Range<rgba>>(str[0]+"-"+str[1]);
	if(str.size() > 2)
	{
		result.max = lexical_cast<Range<rgba>>(str[2]+"-"+str[3]);
	}
	return result;
}
*/

/*
 *	String to Containers
 */

template<>
vector<string> lexical_cast<vector<string>, string>(const string& src)
{
	deque<string> toks = Split(src, "\n{}", ", \t");
	vector<string> result;
	bool indent = false;
	for(uint i = 0; i < toks.size(); ++i)
	{
		if(!indent && i == 0)
		{
			while(toks[i] == " " || toks[i] == "\t")
			{
				++i;
			}
			indent = true;
		}
		if(toks[i] == ",")
		{
			++i;
			while(toks[i] == " " || toks[i] == "\t")
			{
				++i;
			}
		}

		result.push_back(toks[i]);
	}
	return result;
}

template<>
deque<string> lexical_cast<deque<string>, string>(const string& src)
{
	deque<string> toks = Split(src, "\n{}", ", \t");
	deque<string> result;
	bool indent = false;
	for(uint i = 0; i < toks.size(); ++i)
	{
		if(!indent && i == 0)
		{
			while(toks[i] == " " || toks[i] == "\t")
			{
				++i;
			}
			indent = true;
		}
		if(toks[i] == ",")
		{
			++i;
			while(toks[i] == " " || toks[i] == "\t")
			{
				++i;
			}
		}

		result.push_back(toks[i]);
	}
	return result;
}

template<>
string lexical_cast<string, deque<string> >(const deque<string>& src)
{
	string result = "{ ";
	for(uint i = 0; i < src.size(); ++i)
	{
		result.append(src[i]);
		if(i+1 < src.size())
		{
			result.append(", ");
		}
	}
	result.append(" }");
	return result;
}


}
