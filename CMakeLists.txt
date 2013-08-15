
get_filename_component(ROOT "./" REALPATH)
set(LFANT "${ROOT}")
set(CMAKE_MODULE_PATH "${ROOT}/cmake")

include(Config)

if(PLATFORM STREQUAL "android")
	message("On Android, using special gcc, ${CMAKE_C_COMPILER}")
#	set(CMAKE_C_COMPILER "gcc")
#	set(CMAKE_CXX_COMPILER "g++")
else()
	if(UNIX)
	set(CMAKE_C_COMPILER "clang")
	set(CMAKE_CXX_COMPILER "clang++")
	endif()
endif()

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
