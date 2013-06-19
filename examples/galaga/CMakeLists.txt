
if(UNIX)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
endif()

cmake_minimum_required(VERSION 2.8)
project(galaga)

get_filename_component(ROOT "./" REALPATH)
set(LFANT ${ROOT}/lfant)
set(CMAKE_MODULE_PATH ${LFANT}/cmake)

include(Config)

# Source Dir
file(GLOB_RECURSE GAME_SRC
	"src/${PROJECT_NAME}/*.cpp"
	)

file(GLOB LAUNCHER_SRC
	"src/launcher/main.cpp"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	"${LIBRARY_OUTPUT_PATH}/lib"
	# System
#	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${GAME_SRC})
add_executable(launcher ${LAUNCHER_SRC})

# Includes
include_directories(
	"src"
	"${LFANT}/src"
	"${LFANT}/external/glm"
	"${LFANT}/external/boost"
	"${LFANT}/external/glfw/include"
	"${LFANT}/external/raknet/Source"
	"${LFANT}/external/cegui/cegui/include"
	)

target_link_libraries(launcher
	boost
	)

if(PLATFORM STREQUAL "linux")
target_link_libraries(launcher
	dl
	)
endif()

target_link_libraries(${PROJECT_NAME}
	lfant
	)
