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
#set (CMAKE_D_FLAGS_INIT "${DSTDLIB_FLAGS}")
set (CMAKE_D_FLAGS_DEBUG_INIT "-g ${DDOC_FLAGS}")
set (CMAKE_D_FLAGS_MINSIZEREL_INIT "-Os ${DDOC_FLAGS}")
set (CMAKE_D_FLAGS_RELEASE_INIT "-O3 -fomit-frame-pointer -fweb -frelease -finline-functions ${DDOC_FLAGS}")
set (CMAKE_D_FLAGS_RELWITHDEBINFO_INIT "-O2 -g ${DDOC_FLAGS}")
SET (CMAKE_D_CREATE_ASSEMBLY_SOURCE "<CMAKE_D_COMPILER> <FLAGS> -S <SOURCE> -o <ASSEMBLY_SOURCE>")