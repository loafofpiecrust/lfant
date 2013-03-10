project(galaga)

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
	# System
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

add_dependencies(${PROJECT_NAME} sfengine)

# Includes
include_directories(
	"../"
	"${SFE}/src"
	"${SFE}/src/external/glm"
	"${SFE}/src/external/boost"
	"${SFE}/src/external/glfw/include"
	"${SFE}/src/external/raknet/Source"
	)

target_link_libraries(${PROJECT_NAME}
	lfant
	)
