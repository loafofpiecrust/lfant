
if(UNIX)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
endif()

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)
project(galaga)

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

if(PLATFORM STREQUAL "linux")
	if(EXISTS "${PROJECT_SOURCE_DIR}/bin32/linux/lfant/")
	else()
		file(STRINGS "${PROJECT_SOURCE_DIR}/bin32/linux/lfantdir.txt" TEMP_STR)
		message("Project lfant dir: ${TEMP_STR}")
		execute_process(
			COMMAND cd ${PROJECT_SOURCE_DIR};ln -s ${TEMP_STR} bin32/linux/lfant
			)
	endif()
endif()

if(PLATFORM STREQUAL "windows")
	if(EXISTS "${PROJECT_SOURCE_DIR}/bin32/windows/lfant/")
	else()
		file(STRINGS "${PROJECT_SOURCE_DIR}/bin32/windows/lfantdir.txt" TEMP_STR)
		message("cmd /c 'mklink /D \"${PROJECT_SOURCE_DIR}/bin32/windows/lfant\" \"${TEMP_STR}\"'")
		execute_process(
			COMMAND cmd /c 'mklink /D \"${PROJECT_SOURCE_DIR}/bin32/windows/lfant\" \"${TEMP_STR}\"'
	#		RESULT_VARIABLE TEMP_STR
			)
	#	message("Output: '${TEMP_STR}'")
	endif()
endif()

get_filename_component(ROOT "${PROJECT_SOURCE_DIR}/" REALPATH)
set(LFANT ${ROOT}/bin32/${PLATFORM}/lfant)
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
target_link_libraries(${PROJECT_NAME}
	lfant
	)
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
