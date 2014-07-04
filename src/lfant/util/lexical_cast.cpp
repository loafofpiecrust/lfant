
#include <lfant/util/lexical_cast.h>

// Internal
#include <lfant/util/String.h>
#include <lfant/Range.h>
#include <lfant/Console.h>
#include <lfant/Rect.h>

// External
#include <glm/gtx/quaternion.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

using namespace std;

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
	char str[12];
	sprintf(str, "%p", src);
	return str;
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
	char str[12];
	sprintf(str, "%i", src);
	return str;
}

template<>
string lexical_cast<string, unsigned char>(const unsigned char& src)
{
	char str[4];
	sprintf(str, "%u", src);
	return str;
}

template<>
string lexical_cast<string, unsigned short>(const unsigned short& src)
{
	char str[6];
	sprintf(str, "%u", src);
	return str;
}

template<>
string lexical_cast<string, unsigned int>(const unsigned int& src)
{
	char str[12];
	sprintf(str, "%u", src);
	return str;
}

template<>
string lexical_cast<string, unsigned long>(const unsigned long& src)
{
	char str[20];
	sprintf(str, "%lu", src);
	return str;
}

template<>
string lexical_cast<string, unsigned long long>(const unsigned long long& src)
{
	char str[40];
	sprintf(str, "%llu", src);
	return str;
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

string cut_zeroes(string orig)
{
	uint pos = 0;
	string str = orig;
	for(uint i = 0; i < str.size(); ++i)
	{
		if(str[str.size()-1-i] != '0')
		{
			pos = str.size()-i+1;
			break;
		}
	}
	if(pos < str.size())
	{
		str.erase(str.begin()+pos, str.end());
	}
	return str;
}

template<>
string lexical_cast<string, float>(const float& src)
{
	return cut_zeroes(std::to_string(src));
}

template<>
string lexical_cast<string, double>(const double& src)
{
	return cut_zeroes(std::to_string(src));
}

template<>
string lexical_cast<string, vec2>(const vec2& src)
{
	return "("+lexical_cast<string>(src.x)+","+lexical_cast<string>(src.y)+")";
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
string lexical_cast<string, dvec3>(const dvec3& src)
{
	return "("+lexical_cast<string>(src.x)+","+lexical_cast<string>(src.y)+","+
			lexical_cast<string>(src.z)+")";
}

template<>
string lexical_cast<string, bvec3>(const bvec3& src)
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
string lexical_cast<string, u8vec4>(const u8vec4& src)
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
string lexical_cast<string, Range<u8vec4> >(const Range<u8vec4>& src)
{
	return "("+lexical_cast<string>(src.min)+")-("+lexical_cast<string>(src.max)+")";
}

template<>
string lexical_cast<string, Range<vec3>>(const Range<vec3>& src)
{
	return "("+lexical_cast<string>(src.min)+")-("+lexical_cast<string>(src.max)+")";
}

template<>
string lexical_cast<string, Range<Range<vec3>>>(const Range<Range<vec3>>& src)
{
	return "("+lexical_cast<string>(src.start)+")-("+lexical_cast<string>(src.end)+")";
}

template<>
string lexical_cast<string, Range<vec4>>(const Range<vec4>& src)
{
	return "("+lexical_cast<string>(src.min)+")-("+lexical_cast<string>(src.max)+")";
}

template<>
string lexical_cast<string, Range<Range<vec4>>>(const Range<Range<vec4>>& src)
{
	return "("+lexical_cast<string>(src.start)+")-("+lexical_cast<string>(src.end)+")";
}

template<>
string lexical_cast<string, Rect>(const Rect& src)
{
	return "("+lexical_cast<string>(src.x)+","+
		lexical_cast<string>(src.y)+","+
		lexical_cast<string>(src.width)+","+
		lexical_cast<string>(src.height)+")";
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
	std::deque<string> str = lfant::Split(val, " x:,()");
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
	std::deque<string> str = lfant::Split(val, " x:,()");
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
	std::deque<string> str = lfant::Split(val, " x:,()");
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
bvec3 lexical_cast<bvec3, string>(const string& val)
{
	bvec3 result(0);
	std::deque<string> str = lfant::Split(val, " x:,()");
	result.x = lexical_cast<bool>(str[0]);
	if(str.size() > 1)
	{
		result.y = lexical_cast<bool>(str[1]);
		if(str.size() > 2)
		{
			result.z = lexical_cast<bool>(str[2]);
		}
	}
	return result;
}

template<>
vec3 lexical_cast<vec3, string>(const string& val)
{
	vec3 result(0);
	string valstr = val;
/*	for(uint i = 0; i < valstr.size(); ++i)
	{
		if(valstr[i] == -7)
		{
			valstr.erase(i, 1);
		}
	}*/
	std::deque<string> str = lfant::Split(valstr, " x:,()");
	result.x = lexical_cast<float>(str[0]);
	if(str.size() > 1)
	{
		result.y = lexical_cast<float>(str[1]);
		if(str.size() > 2)
		{
			result.z = lexical_cast<float>(str[2]);
		}
	}
	else
	{
		result.y = result.x;
		result.z = result.x;
	}
	return result;
}

template<>
dvec3 lexical_cast<dvec3, string>(const string& val)
{
	dvec3 result(0);
	std::deque<string> str = lfant::Split(val, " x:,()");
	for(int i = 0; i < str.size(); ++i)
	{
		result[i] = lexical_cast<double>(str[i]);
	}
	return result;
}

template<>
u8vec4 lexical_cast<u8vec4, string>(const string& src)
{
	u8vec4 result(0);
	std::deque<string> str = lfant::Split(src, " x,()");
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

template<>
vec4 lexical_cast<vec4, string>(const string& src)
{
	vec4 result(0);
	deque<string> str = lfant::Split(src, " x,()");
	result.r = lexical_cast<float>(str[0]);
	if(str.size() > 1)
	{
		result.g = lexical_cast<float>(str[1]);
		if(str.size() > 2)
		{
			result.b = lexical_cast<float>(str[2]);
			if(str.size() > 3)
			{
				result.a = lexical_cast<float>(str[3]);
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
	deque<string> str = lfant::Split(src, " ,-()");
	result.min = lexical_cast<float>(str[0]);
	if(str.size() > 1)
	{
		result.max = lexical_cast<float>(str[1]);
	}
	return result;
}

template<>
Range<vec3> lexical_cast<Range<vec3>, string>(const string &src)
{
	Range<vec3> result(vec3(0));
	deque<string> str = lfant::Split(src, " -");
	result.min = lexical_cast<vec3>(str[0]);
	if(str.size() > 1)
	{
		result.max = lexical_cast<vec3>(str[1]);
	}
	return result;
}

template<>
Range<vec4> lexical_cast<Range<vec4>, string>(const string &src)
{
	Range<vec4> result(vec4(0));
	deque<string> str = lfant::Split(src, " -");
	result.min = lexical_cast<vec4>(str[0]);
	if(str.size() > 1)
	{
		result.max = lexical_cast<vec4>(str[1]);
	}
	return result;
}

template<>
Range<Range<float>> lexical_cast<Range<Range<float>>, string>(const string& src)
{
	Range<Range<float>> result(0);
	deque<string> str = lfant::Split(src, " ,-()");
	result.min = lexical_cast<Range<float>>(str[0]+"-"+str[1]);
	if(str.size() > 2)
	{
		result.max = lexical_cast<Range<float>>(str[2]+"-"+str[3]);
	}
	return result;
}

template<>
Range<Range<vec4>> lexical_cast<Range<Range<vec4>>, string>(const string& src)
{
	Range<Range<vec4>> result;
	deque<string> str = lfant::Split(src, " -");
	result.min = lexical_cast<Range<vec4>>(str[0]+"-"+str[1]);
	if(str.size() > 2)
	{
		result.max = lexical_cast<Range<vec4>>(str[2]+"-"+str[3]);
	}
	return result;
}


template<>
Range<Range<vec3>> lexical_cast<Range<Range<vec3>>, string>(const string& src)
{
	Range<Range<vec3>> result;
	deque<string> str = lfant::Split(src, " -");
	result.min = lexical_cast<Range<vec3>>(str[0]+"-"+str[1]);
	if(str.size() > 2)
	{
		result.max = lexical_cast<Range<vec3>>(str[2]+"-"+str[3]);
	}
	return result;
}

/*
 *	String to Containers
 */

template<>
string lexical_cast<string, vector<string>>(const vector<string>& src)
{
	string result = "{";
	for(uint i = 0; i < src.size(); ++i)
	{
		result.append(src[i]);
		if(i != src.size()-1)
		{
			result.append(",");
		}
	}
	result.append("}");
	return result;
}

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

static string TrimSpace(const string& str)
{
	std::size_t firstLetter = str.find_first_not_of(" \t\n");
	std::size_t lastLetter = str.find_last_not_of(" \t\n");

	if(firstLetter == string::npos)
	{
		return "";
	}
	else
	{
		return str.substr(firstLetter, lastLetter-firstLetter+1);
	}
}

template<>
deque<string> lexical_cast<deque<string>, string>(const string& src)
{
	deque<string> toks = Split(src, "\n{}[]", ", \t");
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

		if(!toks[i].empty())
		{
			toks[i] = TrimSpace(toks[i]);
		}

		if(!toks[i].empty()) result.push_back(toks[i]);
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
