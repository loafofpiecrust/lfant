project(assimp)

<<<<<<< HEAD
# Source Dir
file(GLOB_RECURSE SRC_FILES
	"code/*.cpp"
	"contrib/ConvertUTF/*.cpp"
	"contrib/unzip/*.c"
	)
=======
# Define here the needed parameters
set (ASSIMP_VERSION_MAJOR 3)
set (ASSIMP_VERSION_MINOR 0)
set (ASSIMP_VERSION_PATCH 1) # subversion revision?
set (ASSIMP_VERSION ${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
set (ASSIMP_SOVERSION 3)
SET ( PROJECT_VERSION "${ASSIMP_VERSION}" )

set(ASSIMP_PACKAGE_VERSION "0" CACHE STRING "the package-specific version used for uploading the sources")

option(ASSIMP_OPT_BUILD_PACKAGES "Set to ON to generate CPack configuration files and packaging targets" OFF)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
set(LIBASSIMP_COMPONENT libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
set(LIBASSIMP-DEV_COMPONENT libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH}-dev)
set(CPACK_COMPONENTS_ALL assimp-bin ${LIBASSIMP_COMPONENT} ${LIBASSIMP-DEV_COMPONENT} assimp-dev)
set(ASSIMP_LIBRARY_SUFFIX "" CACHE STRING "Suffix to append to library names")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-fPIC) # this is a very important switch and some libraries seem now to have it....
  ## hide all not-exported symbols
  add_definitions( -fvisibility=hidden -Wall )
endif()

INCLUDE (FindPkgConfig)
INCLUDE_DIRECTORIES( include )

# If this is an in-source build (CMAKE_SOURCE_DIR == CMAKE_BINARY_DIR),
# write the library/executable files to the respective directories in the
# source tree. During an out-of-source build, however, do not litter this
# directory, since that is probably what the user wanted to avoid.
IF ( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR )
	SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
	SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
ENDIF ( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR )

# Cache these to allow the user to override them manually.
SET( ASSIMP_LIB_INSTALL_DIR "lib" CACHE PATH
	"Path the built library files are installed to." )
SET( ASSIMP_INCLUDE_INSTALL_DIR "include" CACHE PATH
	"Path the header files are installed to." )
SET( ASSIMP_BIN_INSTALL_DIR "bin" CACHE PATH
	"Path the tool executables are installed to." )

SET(ASSIMP_DEBUG_POSTFIX "D" CACHE STRING "Debug Postfitx for lib, samples and tools")

# Allow the user to build a static library
SET ( ASSIMP_BUILD_STATIC_LIB OFF CACHE BOOL
	"Build a static (.a) version of the library"
)

# Generate a pkg-config .pc for the Assimp library.
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/assimp.pc.in" "${PROJECT_BINARY_DIR}/assimp.pc" @ONLY )
INSTALL( FILES "${PROJECT_BINARY_DIR}/assimp.pc" DESTINATION ${ASSIMP_LIB_INSTALL_DIR}/pkgconfig/ COMPONENT ${LIBASSIMP-DEV_COMPONENT})

# cmake configuration files
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assimp-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/assimp-config.cmake" @ONLY IMMEDIATE)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/assimp-config-version.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/assimp-config-version.cmake" @ONLY IMMEDIATE)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/assimp-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/assimp-config-version.cmake" DESTINATION "${ASSIMP_LIB_INSTALL_DIR}/cmake/assimp-${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}" COMPONENT ${LIBASSIMP-DEV_COMPONENT})

# add make uninstall capability
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Globally enbale Boost resp. the Boost workaround – it is also needed by the
# tools which include the Assimp headers.
SET ( ASSIMP_ENABLE_BOOST_WORKAROUND OFF CACHE BOOL
	"If a simple implementation of the used Boost functions is used. Slightly reduces functionality, but enables builds without Boost available."
)
IF ( ASSIMP_ENABLE_BOOST_WORKAROUND )
	INCLUDE_DIRECTORIES( code/BoostWorkaround )
	ADD_DEFINITIONS( -DASSIMP_BUILD_BOOST_WORKAROUND )
	MESSAGE( STATUS "Building a non-boost version of Assimp." )
ELSE ( ASSIMP_ENABLE_BOOST_WORKAROUND )
	SET( Boost_DETAILED_FAILURE_MSG ON )
	SET( Boost_ADDITIONAL_VERSIONS "1.47" "1.47.0" "1.48.0" "1.48" "1.49" "1.49.0" "1.50" "1.50.0" "1.51" "1.51.0" "1.52.0" "1.53.0" "1.54.0")	
	FIND_PACKAGE( Boost )
	IF ( NOT Boost_FOUND )
		MESSAGE( FATAL_ERROR
			"Boost libraries (http://www.boost.org/) not found. "
			"You can build a non-boost version of Assimp with slightly reduced "
      "functionality by specifying -DASSIMP_ENABLE_BOOST_WORKAROUND=ON."
		)
	ENDIF ( NOT Boost_FOUND )

	INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
ENDIF ( ASSIMP_ENABLE_BOOST_WORKAROUND )


SET ( ASSIMP_NO_EXPORT OFF CACHE BOOL
	"Disable Assimp's export functionality." 
)

# Search for external dependencies, and build them from source if not found
# Search for zlib
find_package(ZLIB)
if( NOT ZLIB_FOUND )
  message(STATUS "compiling zlib from souces")
  include(CheckIncludeFile)
  include(CheckTypeSize)
  include(CheckFunctionExists)
  # compile from sources
  add_subdirectory(contrib/zlib)
  set(ZLIB_FOUND 1)
  set(ZLIB_LIBRARIES zlib)
  set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib)
else(NOT ZLIB_FOUND)
  ADD_DEFINITIONS(-DASSIMP_BUILD_NO_OWN_ZLIB)
endif(NOT ZLIB_FOUND)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

# Search for unzip
if (PKG_CONFIG_FOUND)
	PKG_CHECK_MODULES(UNZIP minizip)
endif (PKG_CONFIG_FOUND)

IF ( ASSIMP_NO_EXPORT )
	ADD_DEFINITIONS( -DASSIMP_BUILD_NO_EXPORT)
	MESSAGE( STATUS "Build an import-only version of Assimp." )
ENDIF( ASSIMP_NO_EXPORT )

# if(CMAKE_CL_64)
# 	set(ASSIMP_BUILD_ARCHITECTURE "amd64")
# else(CMAKE_CL_64)
# 	set(ASSIMP_BUILD_ARCHITECTURE "x86")
# endif(CMAKE_CL_64)
SET ( ASSIMP_BUILD_ARCHITECTURE "" CACHE STRING 
	"describe the current architecture."
)
IF    ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
ELSE  ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
	ADD_DEFINITIONS ( -D'ASSIMP_BUILD_ARCHITECTURE="${ASSIMP_BUILD_ARCHITECTURE}"' )
ENDIF ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")

# ${CMAKE_GENERATOR}
SET ( ASSIMP_BUILD_COMPILER "" CACHE STRING 
	"describe the current compiler."
)
IF    ( ASSIMP_BUILD_COMPILER STREQUAL "")
ELSE  ( ASSIMP_BUILD_COMPILER STREQUAL "")
	ADD_DEFINITIONS ( -D'ASSIMP_BUILD_COMPILER="${ASSIMP_BUILD_COMPILER}"' )
ENDIF ( ASSIMP_BUILD_COMPILER STREQUAL "")

MARK_AS_ADVANCED ( ASSIMP_BUILD_ARCHITECTURE ASSIMP_BUILD_COMPILER )


ADD_SUBDIRECTORY( code/ )
SET ( ASSIMP_BUILD_ASSIMP_TOOLS ON CACHE BOOL
	"If the supplementary tools for Assimp are built in addition to the library."
)
IF ( ASSIMP_BUILD_ASSIMP_TOOLS )
	IF ( WIN32 )
		ADD_SUBDIRECTORY( tools/assimp_view/ )
	ENDIF ( WIN32 )
	ADD_SUBDIRECTORY( tools/assimp_cmd/ )
ENDIF ( ASSIMP_BUILD_ASSIMP_TOOLS )

SET ( ASSIMP_BUILD_SAMPLES OFF CACHE BOOL
	"If the official samples are built as well (needs Glut)."
)

IF ( ASSIMP_BUILD_SAMPLES)
	IF ( WIN32 )
		ADD_SUBDIRECTORY( samples/SimpleTexturedOpenGL/ )
	ENDIF ( WIN32 )
	ADD_SUBDIRECTORY( samples/SimpleOpenGL/ )
ENDIF ( ASSIMP_BUILD_SAMPLES )
>>>>>>> 4277413052fea063b7dd53c8e37a78a9fa46d24d

file(GLOB_RECURSE HEADER_FILES
	"include/*.h"
	"code/*.h"
	)

list(REMOVE_ITEM SRC_FILES
	"${PROJECT_SOURCE_DIR}/code/OgreImporter.cpp"
	"${PROJECT_SOURCE_DIR}/code/OgreMaterial.cpp"
	"${PROJECT_SOURCE_DIR}/code/OgreMesh.cpp"
	"${PROJECT_SOURCE_DIR}/code/OgreSkeleton.cpp"
	"${PROJECT_SOURCE_DIR}/code/IRRLoader.cpp"
	"${PROJECT_SOURCE_DIR}/code/IRRMeshLoader.cpp"
	"${PROJECT_SOURCE_DIR}/code/IRRShared.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCBoolean.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCCurve.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCGeometry.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCLoader.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCMaterial.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCOpenings.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCProfile.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCReaderGen.cpp"
	"${PROJECT_SOURCE_DIR}/code/IFCUtil.cpp"
	"${PROJECT_SOURCE_DIR}/code/ColladaParser.cpp"
	"${PROJECT_SOURCE_DIR}/code/ColladaLoader.cpp"
	"${PROJECT_SOURCE_DIR}/code/XGLLoader.cpp"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Includes
include_directories(
	"include"
	"${ROOT}/external/boost"
	)

add_definitions(
	-DASSIMP_BUILD_NO_IRRMESH_IMPORTER
	-DASSIMP_BUILD_NO_IRR_IMPORTER
	-DASSIMP_BUILD_NO_OGRE_IMPORTER
	-DASSIMP_BUILD_NO_IFC_IMPORTER
	-DASSIMP_BUILD_NO_COLLADA_IMPORTER
	-DASSIMP_BUILD_NO_XGL_IMPORTER
	)

target_link_libraries(${PROJECT_NAME}
	rt
	z
	)
