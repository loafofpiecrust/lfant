project(CEGUI)


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

# Source Info
file(GLOB SRC_FILES
	cegui/src/*.cpp
	cegui/src/falagard/*.cpp
	cegui/src/widgets/*.cpp
	cegui/src/WindowRendererSets/Core/*.cpp
	cegui/src/XMLParserModules/TinyXML/*.cpp
	cegui/src/ImageCodecModules/STB/*.cpp
	cegui/src/RendererModules/OpenGL/*.cpp
	cegui/include/CEGUI/tinyxml/*.cpp
	cegui/src/CommonDialogs/*.cpp
	cegui/src/CommonDialogs/ColourPicker/*.cpp
	cegui/src/svg/*.cpp
	)

file(GLOB HEADER_FILES
	cegui/src/*.h
	cegui/src/RendererModules/OpenGL3/*.h
	cegui/include/CEGUI/tinyxml/*.h
	)

# Remove unneeded source files
list(REMOVE_ITEM SRC_FILES
	${PROJECT_SOURCE_DIR}/cegui/src/FribidiVisualMapping.cpp
	${PROJECT_SOURCE_DIR}/cegui/src/MinizipResourceProvider.cpp
	)
if(PLATFORM STREQUAL "windows")
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/cegui/src/MinizipResourceProvider.cpp
		${PROJECT_SOURCE_DIR}/cegui/src/PCRERegexMatcher.cpp
		${PROJECT_SOURCE_DIR}/cegui/src/IconvStringTranscoder.cpp
		)
else()
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/cegui/src/Win32StringTranscoder.cpp
		${PROJECT_SOURCE_DIR}/cegui/src/Win32ClipboardProvider.cpp
		${PROJECT_SOURCE_DIR}/cegui/src/RendererModules/OpenGL/WGLPBTextureTarget.cpp
		)
endif()

if(NOT PLATFORM STREQUAL "macosx")
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/cegui/src/RendererModules/OpenGL/ApplePBTextureTarget.cpp
		)
endif()

if(NOT PLATFORM STREQUAL "linux")
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
	
add_definitions(
	-DCEGUI_STATIC
	-DGLM_FORCE_RADIANS
	)

target_link_libraries(${PROJECT_NAME}
	glew
	freetype
	m
	supc++
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