cmake_minimum_required(VERSION 2.8)
project(sfroot)

get_filename_component(SFE "./" REALPATH)

set(ARCH "x64")
set(ARCH "x64" CACHE STRING "Architecture to build for. (x86/x64)")

if(ARCH STREQUAL "x64")
	set(ARCH "64")
else()
	set(ARCH "32")
endif()

set(PLATFORM "")
if(UNIX)
	if(APPLE)
		set(PLATFORM "osx")
	else(APPLE)
		set(PLATFORM "linux")
	endif(APPLE)
else(UNIX)
	if(WIN32)
		set(PLATFORM "windows")
	endif(WIN32)
endif(UNIX)

if(PLATFORM STREQUAL "")
	message(FATAL_ERROR "This platform is not supported.")
endif()

#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS "-m${ARCH} -fPIC -O3 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -Wno-invalid-offsetof")

# RPathing
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,$ORIGIN -Wl,-z,origin -Wl,--no-undefined")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,$ORIGIN -Wl,-z,origin")

# Subdirs
add_subdirectory(src)
add_subdirectory(examples)
