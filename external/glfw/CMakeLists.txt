project(glfw)

# Source Dir
file(GLOB SRC_FILES
	src/clipboard.c
	src/context.c
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
	
	add_definitions(
		-DWIN32
	)
endif()

if(PLATFORM STREQUAL "macosx")
	file(GLOB SRC_FILES_PLAT
		src/cocoa_*.c
		src/nsgl_*.c
	)
	set(SRC_FILES ${SRC_FILES} ${SRC_FILES_PLAT})
endif()


# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
#add_executable(${PROJECT_NAME} SRC_FILES)

# Includes
include_directories(
	include
	src
	)

add_definitions(
	-DGLFW_BUILD_DLL
	-D_GLFW_HAS_PTHREAD
	-D_GLFW_HAS_GLXGETPROCADDRESS
	)

#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lXext -lX11")

target_link_libraries(${PROJECT_NAME}
	m
	)

if(PLATFORM STREQUAL "linux")
	target_link_libraries(${PROJECT_NAME}
		GL
		GLU
		pthread
		X11
		Xext
		Xrandr
		Xi
		Xxf86vm
		rt
	)
endif()
if(PLATFORM STREQUAL "windows")
	target_link_libraries(${PROJECT_NAME}
		opengl32
		glu32
		winmm
	)
endif()