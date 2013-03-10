
// External

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

}
