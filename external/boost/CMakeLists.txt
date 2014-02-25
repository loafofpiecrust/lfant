project(boost)

# Source Dir
file(GLOB SRC_FILES
	libs/chrono/src/*.cpp
	libs/system/src/*.cpp
	libs/filesystem/src/*.cpp
	libs/random/src/*.cpp
#	libs/thread/src/pthread/*.cpp
#	libs/thread/src/tss_null.cpp
#	libs/thread/src/future.cpp
)

# Output Library Info
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Includes
include_directories(
	.
)