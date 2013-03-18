
#include <lfant/util/lexical_cast.h>

// Internal
#include <lfant/util/String.h>
#include <lfant/Range.h>

// External

namespace lfant
{

/*
 *	Defaults
 */

template<typename Target, typename Source>
Target lexical_cast(const Source& src)
{
	return (Target)src;
}

template<>
string lexical_cast<string, string>(const string& src)
{
	printf("lex-cast str to str '%s'.\n", src.c_str());
	return src;
}

/*
 *	Primitives from string
 */

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
long lexical_cast<long, string>(const string& src)
{
	return atol(src.c_str());
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
string lexical_cast<string, unsigned short>(const unsigned short& src)
{
	return to_string(src);
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
	vector<string> str = lfant::Split(val, " x:,");
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
	vector<string> str = lfant::Split(val, " x:,");
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
	vector<string> str = lfant::Split(val, " x:,");
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
	vector<string> str = lfant::Split(val, " x:,");
	result.x = lexical_cast<float>(str[0]);
	if(str.size() > 1)
	{
		result.y = lexical_cast<float>(str[1]);
		if(str.size() > 2)
		{
			result.z = lexical_cast<float>(str[2]);
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
	vector<string> str = lfant::Split(src, " .:-,");
	result.min = lexical_cast<int>(str[0]);
	if(str.size() > 1)
	{
		result.max = lexical_cast<int>(str[1]);
	}
	return result;
}

}
