#
# CMakeD - CMake module for D Language
#
# Copyright (c) 2007, Selman Ulug <selman.ulug@gmail.com>
#                     Tim Burrell <tim.burrell@gmail.com>
#
# All rights reserved.
#
# See Copyright.txt for details.
#
# Modified from CMake 2.6.5 gcc.cmake
# See http://www.cmake.org/HTML/Copyright.html for details
#
if (CMAKE_PROJECT_SOURCE_DIR)
    set (CMAKE_D_FLAGS_INIT "-I${CMAKE_PROJECT_SOURCE_DIR}")
endif()
#set(CMAKE_D_COMPILE_OBJECT "<CMAKE_D_COMPILER> <FLAGS> -of<OBJECT> -c <SOURCE>")
#set(CMAKE_D_LINK_EXECUTABLE "<CMAKE_D_COMPILER>  <CMAKE_D_LINK_FLAGS> <OBJECTS> -of<TARGET>")
set (CMAKE_D_FLAGS_DEBUG_INIT "-g")
#  SET (CMAKE_D_FLAGS_MINSIZEREL_INIT "-Os ${DDOC_FLAGS}")
set (CMAKE_D_FLAGS_RELEASE_INIT "-O5 -release -inline")
set (CMAKE_D_FLAGS_RELWITHDEBINFO_INIT "-O -g")

#  SET (CMAKE_D_CREATE_PREPROCESSED_SOURCE "<CMAKE_D_COMPILER> <FLAGS> -E <SOURCE> > <PREPROCESSED_SOURCE>")
set (CMAKE_D_CREATE_ASSEMBLY_SOURCE "<CMAKE_D_COMPILER> <FLAGS> -S <SOURCE> -o <ASSEMBLY_SOURCE>")
set (CMAKE_D_LINK_FLAGS "")
#  SET (CMAKE_INCLUDE_SYSTEM_FLAG_D "-isystem ")
