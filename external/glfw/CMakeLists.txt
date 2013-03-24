project(glfw)

# Source Dir
file(GLOB SRC_FILES
	"src/*.c"
	"src/x11/*.c"
	)

file(GLOB_RECURSE HEADER_FILES
	"include/*.h"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	# System
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
#add_executable(${PROJECT_NAME} SRC_FILES)

# Includes
include_directories(
	"include"
	"src"
	)

add_definitions(
	-DGLFW_BUILD_DLL
	-D_GLFW_HAS_PTHREAD
	-D_GLFW_HAS_GLXGETPROCADDRESS
	)

#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lXext -lX11")

target_link_libraries(${PROJECT_NAME}
	:libGL.so
	GLU
	pthread
	X11
	rt
	Xext
	)
