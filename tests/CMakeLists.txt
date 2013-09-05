
if(UNIX)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
endif()

cmake_minimum_required(VERSION 2.8)
project(lfroot)

get_filename_component(ROOT "../" REALPATH)

set(ARCH "x64")
set(ARCH "x64" CACHE STRING "Architecture to build for. (x86/x64)")

option(BUILD_EXAMPLES "Should the examples be built?" ON)

if(ARCH STREQUAL "x64")
	set(ARCH "64")
	set(ARCH_OPTION "x86-64")
else()
	if(ARCH STREQUAL "x86")
		set(ARCH "32")
		set(ARCH_OPTION "x86")
	else()
		message(FATAL_ERROR "The given architecture is not supported.")
	endif()
endif()

set(PLATFORM "")
if(UNIX)
	if(APPLE)
		set(PLATFORM "macosx")
	else()
		set(PLATFORM "linux")
	endif()
else()
	if(WIN32)
		set(PLATFORM "windows")
	endif()
endif()

if(PLATFORM STREQUAL "")
	message(FATAL_ERROR "This platform is not supported.")
endif()

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS "-m${ARCH} -O3 -Wall -D__STRICT_ANSI__")
if(WIN32)
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -Wno-invalid-offsetof -g -Wno-overloaded-virtual")

add_definitions(
	-D__GXX_EXPERIMENTAL_CXX0X__
	)

# RPathing
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,$ORIGIN -Wl,--no-undefined -g")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,$ORIGIN -g")
if(UNIX)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,origin")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,origin")
endif()

set(LIBRARY_OUTPUT_PATH ${ROOT}/tests/bin${ARCH}/${PLATFORM})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

#set(LIBRARY_OUTPUT_PATH "${ROOT}/bin${ARCH}/${PLATFORM}")

add_subdirectory(Containers)
