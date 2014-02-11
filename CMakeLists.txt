

get_filename_component(ROOT "./" REALPATH)
set(LFANT "${ROOT}")
set(CMAKE_MODULE_PATH "${ROOT}/cmake")

if(WIN32)
include(extra/Platform/Windows-Clang-C)
#set(CMAKE_TOOLCHAIN_FILE extra/toolchain/x86_64-windows-mingw-clang)
include(CMakeExtraBootstrap)
endif()

include(Config)

if(PLATFORM STREQUAL "android")
	message("On Android, using special gcc, ${CMAKE_C_COMPILER}")
#	set(CMAKE_C_COMPILER "gcc")
#	set(CMAKE_CXX_COMPILER "g++")
else()
#	if(UNIX)
	set(CMAKE_C_COMPILER "clang")
	set(CMAKE_CXX_COMPILER "clang++")
#	endif()
endif()

if(PLATFORM STREQUAL "windows")

#include(extra/toolchain/i686-windows-mingw-clang)

#if(CMAKE_C_COMPILER STREQUAL "clang")
	message("Windows clang")

	find_path(STDLIB_PATH libstdc++.a)
	set(HACK_FLAGS "-L${MINGW}/lib -L${STDLIB_PATH} -lstdc++")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${HACK_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HACK_FLAGS}")

	include_directories("${MINGW}/include")

#endif()
endif()

message("C++ flags: '${CMAKE_CXX_FLAGS}'")

cmake_minimum_required(VERSION 2.8)
project(lfroot)

option(BUILD_EXAMPLES "Should the examples be built?" ON)

# Subdirs
add_subdirectory(src)
add_subdirectory(external)

#if(BUILD_EXAMPLES)
#	add_subdirectory(examples)
#	add_subdirectory(tests)
#endif()
