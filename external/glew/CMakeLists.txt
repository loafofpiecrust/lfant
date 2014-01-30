project(glew)

# Source Dir
if(NOT ANDROID)
	file(GLOB SRC_FILES
		src/glew.c
		)
else()
	file(GLOB SRC_FILES
		es/src/glew.c
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
	)
endif()

if(PLATFORM STREQUAL "windows")
set_target_properties(${PROJECT_NAME}
	PROPERTIES CMAKE_IMPORT_LIBRARY_SUFFIX " "
	CMAKE_LINK_LIBRARY_SUFFIX a
	)

target_link_libraries(${PROJECT_NAME}
	opengl32
	glu32
	)
#add_definitions(
#	-lopengl32
#	-lglu32
#	)
endif()

if(ANDROID)
add_definitions(
	-DGLEW_ES_ONLY
	-D__WINSCW__
	)
target_link_libraries(${PROJECT_NAME}
	EGL
#	GL
	GLESv2
	)
endif()
