

get_filename_component(ROOT "./" REALPATH)

set(LFANT "${ROOT}")
set(CMAKE_MODULE_PATH "${ROOT}/cmake")

include(Config)

set(EDITOR ON CACHE BOOL "Build the editor?")
option(BUILD_EXAMPLES "Should the examples be built?" ON)

cmake_minimum_required(VERSION 2.8)
project(lfroot)

message("C++ flags: '${CMAKE_CXX_FLAGS}'")

if(COMPILER STREQUAL "clang")
	SET(CMAKE_STATIC_LIBRARY_PREFIX "lib")
	SET(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
endif()

# Subdirs
add_subdirectory(src)
add_subdirectory(external)

#if(BUILD_EXAMPLES)
#	add_subdirectory(examples)
#	add_subdirectory(tests)
#endif()
