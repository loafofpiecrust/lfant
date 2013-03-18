project(angelscript)

# Source Dir
file(GLOB_RECURSE SRC_FILES
	"source/*.cpp"
	"add_on/*.cpp"
	)

file(GLOB_RECURSE HEADER_FILES
	"include/*.h"
	"source/*.h"
	"add_on/*.h"
	)

# Output Library Info
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})

# Includes
include_directories(
	"include"
	)

