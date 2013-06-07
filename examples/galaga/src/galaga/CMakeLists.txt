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
	"${ROOT}/src"
	"${ROOT}/external/glm"
	"${ROOT}/external/boost"
	"${ROOT}/external/glfw/include"
	"${ROOT}/external/raknet/Source"
	"${ROOT}/external/cegui/cegui/include"
	)

target_link_libraries(${PROJECT_NAME}
	lfant
	)
