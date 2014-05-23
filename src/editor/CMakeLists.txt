cmake_minimum_required(VERSION 2.8.11)
project(editor)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	)

file(GLOB UI_FILES
	*.ui
	)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

qt5_wrap_ui(HEADER_UI_FILES ${UI_FILES})

# Source Info
file(GLOB SRC_FILES
	*.cpp
	gui/*.cpp
	)

file(GLOB HEADER_FILES
	*.h
	gui/*.h
	)

#qt5_wrap_cpp(HEADER_FILES ${SRC_FILES})

# Output Library Info
#add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES} ${UI_FILES})

# Dependencies
#add_dependencies(${PROJECT_NAME} ${ROOT}/external)

# Includes Info
include_directories(
	.
	${ROOT}/src
	${ROOT}/external/glm
	${ROOT}/external/boost
	${ROOT}/external/png
	${ROOT}/external/sfml/include
	${ROOT}/external/squirrel/include
	${Qt5Widgets_INCLUDE_DIRS}
	)

add_definitions(
	${Qt5Widgets_DEFINITIONS}
	)

# Linker Options
target_link_libraries(${PROJECT_NAME}
	lfant
	boost
	dl
	png

	Qt5::Core
	Qt5::Widgets
	Qt5::Gui
	${Qt5Widgets_LIBRARIES}
	)
