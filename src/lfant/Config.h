/**
 *	Game versioning
 */
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
//#define VERSION_PATCH
#define VERSION #VERSION_MAJOR "." #VERSION_MINOR

#define HAVE_VERSION(major,minor) (VERSION_MINOR >= minor && VERSION_MAJOR == major) || (VERSION_MAJOR > major)

// Detect platform if not specified
#if !WINDOWS && !LINUX && !OSX
#	ifdef _WIN32
#		define WINDOWS 1
#	elif __linux
#		define LINUX 1
#	elif _APPLE_
#		define OSX 1
#	endif
#endif

// Platform Preprocessor
#if OSX || LINUX
#	define UNIX 1
#endif

#if WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <conio.h>
#	define GLFWCALL __stdcall
#elif UNIX
#	define ROOTXP extern "C++"
#	define GLFWCALL
#	include <unistd.h>
#endif

// General Preprocessor
#define GLFW_DLL 1

/**
 *	Gives a null type for non-pointers
 *	@todo Make this a class so type specification is not needed.
 */
#define null(type) *((type*)nullptr)
#define GLM_SWIZZLE 1
#define GLM_FORCE_RADIANS 1
#define BOOST_EXCEPTION_DISABLE
//#define __GXX_EXPERIMENTAL_CXX0X__
#ifdef __GXX_EXPERIMENTAL_CXX0X__
#	define GLM_FORCE_CXX11
#endif
