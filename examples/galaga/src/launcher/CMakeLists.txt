project(launcher)

# Source Dir
file(GLOB SRC_FILES
	"./*.cpp"
	)

file(GLOB HEADER_FILES
	"./*.h"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	)

# Output Library Info
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# Includes
include_directories(
	"."
	"${SFE}/external/boost"
	"../"
	"${SFE}/src"
	"${SFE}/external/glm"
	)

target_link_libraries(${PROJECT_NAME}
	dl
	)
