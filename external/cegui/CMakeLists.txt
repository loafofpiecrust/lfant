project(CEGUI)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#include(CEGUIMacros)
#include(CMakeDependentOption)

set(CMAKE_BUILD_TYPE Release)

##############################################
#		Versioning
##############################################
set(CEGUI_VERSION_MAJOR 9999)
set(CEGUI_VERSION_MINOR 0)
set(CEGUI_VERSION_PATCH 0)
set(CEGUI_VERSION ${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}.${CEGUI_VERSION_PATCH} )

set(CEGUI_ABI_CURRENT 2)
set(CEGUI_ABI_REVISION 0)
set(CEGUI_ABI_AGE 0)
set(CEGUI_ABI_VERSION ${CEGUI_ABI_CURRENT}.${CEGUI_ABI_REVISION}.${CEGUI_ABI_AGE} )

set(CEGUI_STRING_CLASS 2)
set(CEGUI_HAS_MINIZIP_RESOURCE_PROVIDER OFF)
set(CEGUI_USE_MINIBIDI OFF)
set(CEGUI_BIDI_SUPPORT OFF)
set(CEGUI_HAS_DEFAULT_LOGGER ON)
set(CEGUI_HAS_FREETYPE ON)
set(CEGUI_TINYXML_HAS_2_6_API ON)
set(CEGUI_STATIC ON)

configure_file( "${PROJECT_SOURCE_DIR}/cegui/include/CEGUI/Version.h.in" "${PROJECT_SOURCE_DIR}/cegui/include/CEGUI/Version.h" )
configure_file( "${PROJECT_SOURCE_DIR}/cegui/include/CEGUI/Config.h.in" "${PROJECT_SOURCE_DIR}/cegui/include/CEGUI/Config.h" )

#add_subdirectory(cegui/src)
#add_subdirectory(cegui/src/RendererModules)
#add_subdirectory(cegui/src/XMLParserModules)
#add_subdirectory(cegui/src/ImageCodecModules)
#add_subdirectory(cegui/src/WindowRendererSets)
#add_subdirectory(cegui/src/ScriptModules)
#add_subdirectory(cegui/src/CommonDialogs)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Source Info
file(GLOB SRC_FILES
	"cegui/src/*.cpp"
	"cegui/src/falagard/*.cpp"
	"cegui/src/widgets/*.cpp"
	"cegui/src/WindowRendererSets/Core/*.cpp"
	"cegui/src/XMLParserModules/TinyXML/*.cpp"
	"cegui/src/ImageCodecModules/STB/*.cpp"
	"cegui/src/RendererModules/OpenGL/*.cpp"
	"cegui/include/CEGUI/tinyxml/*.cpp"
	"cegui/src/CommonDialogs/*.cpp"
	"cegui/src/CommonDialogs/ColourPicker/*.cpp"
	)

file(GLOB HEADER_FILES
	"cegui/src/*.h"
	"cegui/src/RendererModules/OpenGL3/*.h"
	"cegui/include/CEGUI/tinyxml/*.h"
	)

# Remove unneeded source files
list(REMOVE_ITEM SRC_FILES FribidiVisualMapping.cpp)
if(PLATFORM STREQUAL "windows")
	list(REMOVE_ITEM SRC_FILES 
		${PROJECT_SOURCE_DIR}/cegui/src/IconvStringTranscoder.cpp
		${PROJECT_SOURCE_DIR}/cegui/src/MinizipResourceProvider.cpp
		${PROJECT_SOURCE_DIR}/cegui/src/PCRERegexMatcher.cpp
		)
else()
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/cegui/src/Win32StringTranscoder.cpp
		${PROJECT_SOURCE_DIR}/cegui/src/RendererModules/OpenGL/WGLPBTextureTarget.cpp
		)
endif()

if(PLATFORM STREQUAL "osx")
else()
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/cegui/src/RendererModules/OpenGL/ApplePBTextureTarget.cpp
		)
endif()

if(PLATFORM STREQUAL "linux")
else()
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/cegui/src/RendererModules/OpenGL/GLXPBTextureTarget.cpp
		)
endif()

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Includes Info
include_directories(
	"cegui/include"
	"../boost"
	"../glm"
	"../glew/include"
	"cegui/include/CEGUI/freetype"
	"cegui/include/CEGUI/tinyxml"
	)

target_link_libraries(${PROJECT_NAME}
	glew
	freetype
	)

if(PLATFORM STREQUAL "linux")
	target_link_libraries(${PROJECT_NAME}
		pcre
		dl
		)
endif()

if(PLATFORM STREQUAL "windows")
	target_link_libraries(${PROJECT_NAME}
		winmm
		)
endif()

add_definitions(
	-DCEGUI_STATIC
	)
