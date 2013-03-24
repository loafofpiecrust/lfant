
// External
#include <lfant/util/lexical_cast.h>

// Internal
#include <lfant/TypeInfo.h>

namespace lfant
{

string ParamType()
{
	return "";
}

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
	vector<string> toks = Split(type, ":");
	return toks[toks.size()-1];
}

}

