project(squirrel)

# Source Dir
file(GLOB SRC_FILES
	squirrel/*.cpp
	sqstdlib/*.cpp
	sqimport/*.cpp
#	sqratthread/*.cpp
	)

# Output Library Info
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Includes
include_directories(
	./include
	)

