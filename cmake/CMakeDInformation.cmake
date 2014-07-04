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
# Modified from CMake 2.6.5 CMakeCInformation.cmake
# See http://www.cmake.org/HTML/Copyright.html for details
#

# This file sets the basic flags for the D language in CMake.
# It also loads the available platform file for the system-compiler
# if it exists.

message("dflags (start DInformation): ${CMAKE_D_COMPILER}")

get_filename_component(CMAKE_BASE_NAME ${CMAKE_D_COMPILER} NAME_WE)
if (CMAKE_BASE_NAME STREQUAL "dmd")
    set (CMAKE_COMPILER_IS_DMD 1)
elseif (CMAKE_BASE_NAME STREQUAL "ldmd2")
    set (CMAKE_COMPILER_IS_LDC 1)
elseif(CMAKE_BASE_NAME STREQUAL "gdc")
    set(CMAKE_COMPILER_IS_GDC 1)
else()
    message(FATAL_ERROR "Unknown D compiler : ${CMAKE_BASE_NAME} does not belong to {dmd, gdc, ldmd2} ")
endif()

set(CMAKE_SYSTEM_AND_D_COMPILER_INFO_FILE 
  ${CMAKE_ROOT}/Modules/Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_BASE_NAME}.cmake)
include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_BASE_NAME} OPTIONAL)

# This should be included before the _INIT variables are
# used to initialize the cache.  Since the rule variables 
# have if blocks on them, users can still define them here.
# But, it should still be after the platform file so changes can
# be made to those values.

IF(CMAKE_USER_MAKE_RULES_OVERRIDE)
   INCLUDE(${CMAKE_USER_MAKE_RULES_OVERRIDE})
ENDIF(CMAKE_USER_MAKE_RULES_OVERRIDE)

IF(CMAKE_USER_MAKE_RULES_OVERRIDE_D)
   INCLUDE(${CMAKE_USER_MAKE_RULES_OVERRIDE_D})
ENDIF(CMAKE_USER_MAKE_RULES_OVERRIDE_D)

# for most systems a module is the same as a shared library
# so unless the variable CMAKE_MODULE_EXISTS is set just
# copy the values from the LIBRARY variables
IF(NOT CMAKE_MODULE_EXISTS)
  SET(CMAKE_SHARED_MODULE_D_FLAGS ${CMAKE_SHARED_LIBRARY_D_FLAGS})
  SET(CMAKE_SHARED_MODULE_CREATE_D_FLAGS ${CMAKE_SHARED_LIBRARY_CREATE_D_FLAGS})
ENDIF(NOT CMAKE_MODULE_EXISTS)

if (NOT CMAKE_COMPILER_IS_LDC)
  set (CMAKE_D_FLAGS "$ENV{CFLAGS} ${CMAKE_D_FLAGS_INIT}" CACHE STRING "Flags for D compiler.")
else(NOT CMAKE_COMPILER_IS_LDC)
    set (CMAKE_D_FLAGS "-I.") # Hack for -I empty not functioning with LDC
endif(NOT CMAKE_COMPILER_IS_LDC)   

  
IF(NOT CMAKE_NOT_USING_CONFIG_FLAGS)
# default build type is none
  IF(NOT CMAKE_NO_BUILD_TYPE)
    SET (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_INIT} CACHE STRING 
      "Choose the type of build, options are: None(CMAKE_D_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
  ENDIF(NOT CMAKE_NO_BUILD_TYPE)
  SET (CMAKE_D_FLAGS_DEBUG "${CMAKE_D_FLAGS_DEBUG_INIT}" CACHE STRING
    "Flags used by the compiler during debug builds.")
  SET (CMAKE_D_FLAGS_MINSIZEREL "${CMAKE_D_FLAGS_MINSIZEREL_INIT}" CACHE STRING
    "Flags used by the compiler during release minsize builds.")
  SET (CMAKE_D_FLAGS_RELEASE "${CMAKE_D_FLAGS_RELEASE_INIT}" CACHE STRING
    "Flags used by the compiler during release builds (/MD /Ob1 /Oi /Ot /Oy /Gs will produce slightly less optimized but smaller files).")
  SET (CMAKE_D_FLAGS_RELWITHDEBINFO "${CMAKE_D_FLAGS_RELWITHDEBINFO_INIT}" CACHE STRING
    "Flags used by the compiler during Release with Debug Info builds.")
ENDIF(NOT CMAKE_NOT_USING_CONFIG_FLAGS)

IF(CMAKE_D_STANDARD_LIBRARIES_INIT)
  SET(CMAKE_D_STANDARD_LIBRARIES "${CMAKE_D_STANDARD_LIBRARIES_INIT}"
    CACHE STRING "Libraries linked by defalut with all D applications.")
  MARK_AS_ADVANCED(CMAKE_D_STANDARD_LIBRARIES)
ENDIF(CMAKE_D_STANDARD_LIBRARIES_INIT)

INCLUDE(CMakeCommonLanguageInclude)

# now define the following rule variables

# CMAKE_D_CREATE_SHARED_LIBRARY
# CMAKE_D_CREATE_SHARED_MODULE
# CMAKE_D_CREATE_STATIC_LIBRARY
# CMAKE_D_COMPILE_OBJECT
# CMAKE_D_LINK_EXECUTABLE

# variables supplied by the generator at use time
# <TARGET>
# <TARGET_BASE> the target without the suffix
# <OBJECTS>
# <OBJECT>
# <LINK_LIBRARIES>
# <FLAGS>
# <LINK_FLAGS>

# D compiler information
# <CMAKE_D_COMPILER>  
# <CMAKE_SHARED_LIBRARY_CREATE_D_FLAGS>
# <CMAKE_SHARED_MODULE_CREATE_D_FLAGS>
# <CMAKE_D_LINK_FLAGS>

# Static library tools
# <CMAKE_AR> 
# <CMAKE_RANLIB>

#set(CMAKE_D_VERSION_FLAG "-fversion=")

set(CMAKE_SHARED_LIBRARY_D_FLAGS "-fPIC")
set(CMAKE_SHARED_LIBRARY_CREATE_D_FLAGS "-shared")
set(CMAKE_INCLUDE_FLAG_D "-I")       # -I
set(CMAKE_INCLUDE_FLAG_D_SEP "")     # , or empty

if(CMAKE_COMPILER_IS_GDC)
  set(CMAKE_OUTPUT_D_FLAG "-o ")
  set(CMAKE_LIBRARY_PATH_FLAG "-L")
  set(CMAKE_LINK_LIBRARY_FLAG "-l")
elseif(CMAKE_COMPILER_IS_DMD OR CMAKE_COMPILER_IS_LDC)
  set(CMAKE_OUTPUT_D_FLAG "-of")
  set(CMAKE_LIBRARY_PATH_FLAG "-L-L")
  set(CMAKE_LINK_LIBRARY_FLAG "-L-l")
endif(CMAKE_COMPILER_IS_GDC)

# create a D shared library
if(NOT CMAKE_D_CREATE_SHARED_LIBRARY)
  	 set(CMAKE_D_CREATE_SHARED_LIBRARY
  	    "<CMAKE_D_COMPILER> <CMAKE_SHARED_LIBRARY_D_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_D_FLAGS> <CMAKE_SHARED_LIBRARY_SONAME_D_FLAG><TARGET_SONAME> ${CMAKE_OUTPUT_D_FLAG}<TARGET> <OBJECTS> <LINK_LIBRARIES>")
endif(NOT CMAKE_D_CREATE_SHARED_LIBRARY)

# create a D shared module just copy the shared library rule
IF(NOT CMAKE_D_CREATE_SHARED_MODULE)
  SET(CMAKE_D_CREATE_SHARED_MODULE ${CMAKE_D_CREATE_SHARED_LIBRARY})
ENDIF(NOT CMAKE_D_CREATE_SHARED_MODULE)

# create a D static library
if(NOT CMAKE_D_CREATE_STATIC_LIBRARY)
  set(CMAKE_D_CREATE_STATIC_LIBRARY "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS> " "<CMAKE_RANLIB> <TARGET> ")
endif(NOT CMAKE_D_CREATE_STATIC_LIBRARY)

# compile a D file into an object file
if(NOT CMAKE_D_COMPILE_OBJECT)
    set(CMAKE_D_COMPILE_OBJECT "<CMAKE_D_COMPILER> <FLAGS> ${CMAKE_OUTPUT_D_FLAG}<OBJECT> -c <SOURCE>")
endif(NOT CMAKE_D_COMPILE_OBJECT)

# link D files into executable 
if(NOT CMAKE_D_LINK_EXECUTABLE)
   set(CMAKE_D_LINK_EXECUTABLE
      "<CMAKE_D_COMPILER> <FLAGS> <CMAKE_D_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> ${CMAKE_OUTPUT_D_FLAG}<TARGET> <LINK_LIBRARIES> ${DSTDLIB_FLAGS} ${DSTDLIB_TYPE}")
endif(NOT CMAKE_D_LINK_EXECUTABLE)

MARK_AS_ADVANCED(
CMAKE_D_FLAGS
CMAKE_D_FLAGS_DEBUG
CMAKE_D_FLAGS_MINSIZEREL
CMAKE_D_FLAGS_RELEASE
CMAKE_D_FLAGS_RELWITHDEBINFO
)
SET(CMAKE_D_INFORMATION_LOADED 1)

message("dflags (end DInformation): ${CMAKE_D_FLAGS}") 