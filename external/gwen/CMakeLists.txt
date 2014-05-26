project(gwen)

# Source Dir
file(GLOB SRC_FILES
	src/*.*
	src/Controls/*
	src/Platforms/*
	)

# Output Library Info
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Includes
include_directories(
	include
	../glew/include
	)

