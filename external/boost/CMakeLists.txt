project(boost)

# Source Dir
file(GLOB SRC_FILES
	libs/chrono/src/*.cpp
	libs/system/src/*.cpp
	libs/filesystem/src/*.cpp
	libs/random/src/*.cpp
#	libs/thread/src/pthread/*.cpp
	libs/thread/src/tss_null.cpp
	libs/thread/src/future.cpp
	)

if(PLATFORM STREQUAL "linux")
	file(GLOB SRC_FILES_PLAT
		libs/thread/src/pthread/*.cpp
		)
	set(SRC_FILES ${SRC_FILES} ${SRC_FILES_PLAT})
endif()

if(PLATFORM STREQUAL "windows")
	file(GLOB SRC_FILES_PLAT
		libs/thread/src/win32/*.cpp
		)
	set(SRC_FILES ${SRC_FILES} ${SRC_FILES_PLAT})
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/boost/")
	execute_process(
		COMMAND svn update boost
		)
	execute_process(
		COMMAND svn update libs
		)
#	message("We have boost dir")
else()
	execute_process(
		COMMAND svn co http://svn.boost.org/svn/boost/trunk/boost boost
		)
	execute_process(
		COMMAND svn co http://svn.boost.org/svn/boost/trunk/libs libs
		)
#	message("No boost dir")
endif()

# Output Library Info
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Includes
include_directories(
	.
	)
