//#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <boost/extension/shared_library.hpp>
#include <boost/function.hpp>

#if WIN32
#	define EXT ".dll"
#	include <windows.h>
#elif __APPLE__
#	define EXT ".dylib"
#	define UNIX 1
#elif __linux
#	define EXT ".so"
#	define UNIX 1
#endif

#if UNIX
#	include <unistd.h>
#endif

using namespace boost::extensions;
using namespace std;

shared_library loadlib( std::string name )
{
	shared_library lib( name, true );
	if (!lib.open())
	{
		cout << "Failed to load " << name << "\n" << dlerror() << "\n";
	}
	return lib;
}

int main( void )
{
	//Launch();
	shared_library lib = loadlib( "libprojectName" EXT );
	boost::function<void()> func = lib.get<void>( "Launch" );
	if (func)
	{
		cout << "Succeeded to load game\n";
		func();
	}
	else
	{
		cout << "Couldn't find Launch function\n";
	}

	return 0;
}
