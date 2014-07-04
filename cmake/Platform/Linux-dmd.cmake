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

IF(CMAKE_D_BUILD_DOCS)
	SET(DDOC_FLAGS "-D -Dddocumentation")
	#FOREACH(item ${CMAKE_D_DDOC_FILES})
	#	SET(DDOC_FLAGS "${DDOC_FLAGS} ${item}")
	#ENDFOREACH(item)
ENDIF(CMAKE_D_BUILD_DOCS)

#set (CMAKE_D_FLAGS_INIT "-I${CMAKE_PROJECT_SOURCE_DIR}")
set (CMAKE_D_FLAGS_DEBUG_INIT "-g ${DDOC_FLAGS}")
set (CMAKE_D_FLAGS_RELEASE_INIT "-O -release -inline ${DDOC_FLAGS}")
set (CMAKE_D_FLAGS_RELWITHDEBINFO_INIT "-O -g ${DDOC_FLAGS}")

set (CMAKE_D_CREATE_ASSEMBLY_SOURCE "<CMAKE_D_COMPILER> <FLAGS> -S <SOURCE> -o <ASSEMBLY_SOURCE>")
#set (CMAKE_D_LINK_FLAGS "")
#  SET (CMAKE_INCLUDE_SYSTEM_FLAG_D "-isystem ")
