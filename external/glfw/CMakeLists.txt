project(glfw)

# Source Dir
file(GLOB SRC_FILES
	src/context.c
	src/clipboard.c
	src/gamma.c
	src/init.c
	src/input.c
	src/joystick.c
	src/monitor.c
	src/time.c
	src/window.c
	)

if(PLATFORM STREQUAL "linux")
	file(GLOB SRC_FILES_PLAT
		src/glx_*.c
		src/x11_*.c
		)
	list(APPEND SRC_FILES ${SRC_FILES_PLAT})
endif()

if(PLATFORM STREQUAL "windows")
	file(GLOB SRC_FILES_PLAT
		src/wgl_*.c
		src/win32_*.c
		)
	list(APPEND SRC_FILES ${SRC_FILES_PLAT})
endif()

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

if(PLATFORM STREQUAL "linux")
target_link_libraries(${PROJECT_NAME}
	:libGL.so
	GLU
	pthread
	X11
	rt
	Xext
	)
endif()

if(PLATFORM STREQUAL "windows")
target_link_libraries(${PROJECT_NAME}
	opengl32
	glu32
	winmm
	)
endif()