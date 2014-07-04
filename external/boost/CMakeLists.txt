project(boost)

# Source Dir
file(GLOB SRC_FILES
	libs/chrono/src/*.cpp
	libs/system/src/*.cpp
	libs/filesystem/src/*.cpp
	libs/random/src/*.cpp
)

# Output Library Info
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Includes
include_directories(
	.
)