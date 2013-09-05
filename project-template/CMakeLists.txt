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

get_filename_component(ROOT "./" REALPATH)

file(STRINGS "${ROOT}/bin32/${PLATFORM}/lfantdir.txt" LFANT)
set(CMAKE_MODULE_PATH ${LFANT}/cmake)

include(Config)

if(UNIX AND NOT ANDROID)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
endif()

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)
project(projectName)

# Source Dir
file(GLOB_RECURSE GAME_SRC
	"src/${PROJECT_NAME}/*.cpp"
	)

if(ANDROID)
file(GLOB LAUNCHER_SRC
	"src/launcher/android/main.cpp"
	)
else()
file(GLOB LAUNCHER_SRC
	"src/launcher/main.cpp"
	)
endif()

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	"${LIBRARY_OUTPUT_PATH}/lib"
	# System
#	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${GAME_SRC})
target_link_libraries(${PROJECT_NAME}
	lfant
	)

if(!ANDROID)
add_executable(launcher ${LAUNCHER_SRC})
target_link_libraries(launcher
	lfant
	)
endif()

# Includes
include_directories(
	src
	${LFANT}/src
	${LFANT}/external/glm
	${LFANT}/external/boost
	${LFANT}/external/glew/include
	${LFANT}/external/glfw/include
	${LFANT}/external/cegui/cegui/include
	${LFANT}/external/squirrel/include
	)

if(!ANDROID)
target_link_libraries(launcher
	boost
	)

if(PLATFORM STREQUAL "linux")
	target_link_libraries(launcher
		dl
		)
endif()
endif()
