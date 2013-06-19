
project(galaga)

# Source Dir
file(GLOB SRC_FILES
	"*.cpp"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	# System
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Output Library Info
add_executable(${PROJECT_NAME} ${SRC_FILES})

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