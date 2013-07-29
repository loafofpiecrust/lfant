project(glew)

# Source Dir
if(NOT ANDROID)
file(GLOB_RECURSE SRC_FILES
	src/glew.c
	)
else()
file(GLOB_RECURSE SRC_FILES
	src/es/glew.c
	)
endif()

# Library Search Paths
link_directories(
	${LIBRARY_OUTPUT_PATH}
	# System
	/usr/lib/x86_64-linux-gnu/mesa
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Includes
if(NOT ANDROID)
include_directories(
	include
	)
else()
include_directories(
	es/include
	)
endif()

add_definitions(
	-DGLEW_BUILD_DLL
	-DGLEW_NO_GLU
	)

if(PLATFORM STREQUAL "linux")
	target_link_libraries(${PROJECT_NAME}
		GL
		GLU
		pthread
		X11
		stdc++
		)
endif()

if(PLATFORM STREQUAL "windows")
	target_link_libraries(${PROJECT_NAME}
		glu32
		pthread
		gdi32
		user32
		kernel32
		opengl32
		)
endif()
