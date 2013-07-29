project(bullet)

link_directories(
	${LIBRARY_OUTPUT_PATH}
	# System
	/usr/lib/x86_64-linux-gnu/mesa
	)

# Source Dir
file(GLOB_RECURSE SRC_FILES
	src/*.cpp
	)

if(EXISTS "${PROJECT_SOURCE_DIR}/src")
#	execute_process(
#		COMMAND svn update src
#		)
else()
#	execute_process(
#		COMMAND svn co http://svn.boost.org/svn/boost/trunk/libs src
#		)
endif()

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Includes
include_directories(
	src
)

target_link_libraries(${PROJECT_NAME}
	OpenCL
	)