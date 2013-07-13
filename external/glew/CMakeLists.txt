project(glew)

# Source Dir
file(GLOB_RECURSE SRC_FILES
	"src/glew.c"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	# System
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Includes
include_directories("include")

add_definitions(
	-DGLEW_BUILD_DLL
	)

if(PLATFORM STREQUAL "linux")
target_link_libraries(${PROJECT_NAME}
	GL
	GLU
	pthread
	X11
	)
endif()

if(PLATFORM STREQUAL "windows")
target_link_libraries(${PROJECT_NAME}
	opengl32
	glu32
	)
endif()