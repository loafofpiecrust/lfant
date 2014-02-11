

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
	if(UNIX)
	set(CMAKE_C_COMPILER "clang")
	set(CMAKE_CXX_COMPILER "clang++")
	endif()
endif()

cmake_minimum_required(VERSION 2.8)
project(lfroot)

if(PLATFORM STREQUAL "windows" AND CMAKE_C_COMPILER STREQUAL "clang")

	message("Windows clang")

	find_library(MINGW_ARCH_DIR libglu32.a)
	get_filename_component(MINGW_ARCH_DIR "${MINGW_ARCH_DIR}" PATH)
	set(MINGW_ARCH_DIR "${MINGW_ARCH_DIR}/..")

	get_filename_component(MINGW_ROOT "${MINGW_ARCH_DIR}/.." REALPATH)

	file(GLOB MINGW_VER_DIR "${MINGW_ROOT}/lib/gcc/*/*/libgcc.a")
	get_filename_component(MINGW_VER_DIR "${MINGW_VER_DIR}" PATH)

	message("root dir: ${MINGW_ROOT}, ver_dir: ${MINGW_VER_DIR}")

	link_directories(
		${MINGW_ARCH_DIR}/lib
		${MINGW_ARCH_DIR}/lib32
		${MINGW_VER_DIR}
	)

	include_directories(
		${MINGW_ARCH_DIR}/include
		${MINGW_ARCH_DIR}/include/c++
	)

endif()

message("C++ flags: '${CMAKE_CXX_FLAGS}'")

if(COMPILER STREQUAL "clang")
	SET(CMAKE_STATIC_LIBRARY_PREFIX "lib")
	SET(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
endif()

option(BUILD_EXAMPLES "Should the examples be built?" ON)

# Subdirs
add_subdirectory(src)
add_subdirectory(external)

#if(BUILD_EXAMPLES)
#	add_subdirectory(examples)
#	add_subdirectory(tests)
#endif()
