
#include <iostream>
#include <string>
#include <boost/extension/shared_library.hpp>
#include <boost/function.hpp>

using namespace std;
using namespace boost::extensions;

#if defined(WIN32)
const string ext = ".dll";
#elif defined(__linux)
const string ext = ".so";
#elif defined(__APPLE__)
const string ext = ".dylib";
#endif

shared_library LoadLibrary(string name)
{
	shared_library lib {name.c_str()};
	lib.open();
	if(!lib.is_open())
	{
		std::cout << "Couldn't load library.\n";
		exit(0);
	}
	return lib;
}

int main()
{
	shared_library lib = LoadLibrary("libgalaga"+ext);
	boost::function<void()> func = lib.get<void>("Launch");
	if(!func)
	{
		std::cout << "Couldn't find function.\n";
	}
	else
	{
		func();
	}
	return 0;
}