project(lfant)

set(GUI gwen)

# Source Info
file(GLOB SRC_FILES
	*.cpp
	physics/*.cpp
	util/*.cpp
	net/*.cpp
	net/tcp/*.cpp
	platform/${PLATFORM}/*.cpp
	gui/*.cpp
	gui/${GUI}/*.cpp
	*.h
	)

#file(GLOB_RECURSE SRC_FILES_REC
#	*.h
#	)
#list(APPEND SRC_FILES "${SRC_FILES_REC}")

if(PLATFORM STREQUAL "android")
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/OpenCL.cpp
		)
	file(GLOB_RECURSE SRC_FILES_PLAT
		platform/mobile/*.cpp
		)
	list(APPEND SRC_FILES "${SRC_FILES_PLAT}")
else()
	file(GLOB_RECURSE SRC_FILES_PLAT
		platform/desktop/*.cpp
		)
	list(APPEND SRC_FILES "${SRC_FILES_PLAT}")
endif()


list(REMOVE_ITEM SRC_FILES
    ${PROJECT_SOURCE_DIR}/Sound.cpp
	${PROJECT_SOURCE_DIR}/net/udp/*.cpp
	${PROJECT_SOURCE_DIR}/gui/gwen/SFML2.cpp
	)

set(INCLUDES "")
set(LIBS "")

#add_subdirectory(gui/gwen)

message(STATUS "lfant utput: ${ROOT}/bin/${ARCH}/${PLATFORM}")

link_directories(
	${PROJECT_OUTPUT_DIR}
	${ROOT}/bin/${ARCH}/${PLATFORM}
)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Includes Info
include_directories(
	../
	${ROOT}/external/assimp/include
	${ROOT}/external/glm
	${ROOT}/external/boost
	${ROOT}/external/bullet/src
	${ROOT}/external/glfw/include
	${ROOT}/external/openal/include
	${ROOT}/external/squirrel/include
	${ROOT}/external/irrklang/include
	${ROOT}/external/gwen/include
	${ROOT}/external/${GUI}/include
	${ROOT}/external/freetype/include
	${ROOT}/external/AntTweakBar/include
	${ROOT}/external/yaml/include
)


set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
#cotire(${PROJECT_NAME})

# Linker Options
target_link_libraries(${PROJECT_NAME} ${LIBS}
	assimp
	boost
	bullet
	squirrel
#	freeimage
	freetype
#	IrrKlang
	yaml
	${GUI}
	)

if(NOT ANDROID)
	include_directories(
		${ROOT}/external/glew/include
		)

	target_link_libraries(${PROJECT_NAME}
		glfw
	#	OpenCL
		glew

	#	AntTweakBar
		)
else()
	include_directories(
		${ROOT}/external/glew/es/include
		)

	target_link_libraries(${PROJECT_NAME}
		android
		log
		EGL
		GLESv2
		)
endif()


if(PLATFORM STREQUAL "linux")
	target_link_libraries(${PROJECT_NAME}
		rt
		dl
		openal
		OpenCL
	)
endif()

if(PLATFORM STREQUAL "windows")
	target_link_libraries(${PROJECT_NAME}
		":C:/Windows/System32/OpenAL32.dll"
		":C:/Windows/System32/OpenCL.dll"
	#	:FreeImage.lib
		wsock32
		ws2_32
	)
endif()
