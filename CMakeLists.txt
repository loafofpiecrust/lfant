
if(UNIX)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
endif()

cmake_minimum_required(VERSION 2.8.10)
project(lfroot)

get_filename_component(ROOT "./" REALPATH)
set(CMAKE_MODULE_PATH "${ROOT}/cmake")

include(Config)

option(BUILD_EXAMPLES "Should the examples be built?" ON)

# Subdirs
add_subdirectory(src)
add_subdirectory(external)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
#	add_subdirectory(tests)
endif()
