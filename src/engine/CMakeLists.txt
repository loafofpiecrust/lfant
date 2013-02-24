project(sfengine)

# Versioning
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

set(INCLUDE_DIR ".")
set(SOURCE_DIR ".")

# Configured Header for Macro'd settings, like versioning
configure_file(
	"${PROJECT_SOURCE_DIR}/Config.hpp.in"
	"${PROJECT_SOURCE_DIR}/Config.hpp"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	)

# Source Info
file(GLOB_RECURSE SRC_FILES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${INCLUDE_DIR}/*.hpp")

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
#add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# Dependencies
add_dependencies(${PROJECT_NAME} ../external)

# Includes Info
include_directories(
	"${INCLUDE_DIR}"
	"${SFE}/src/external/fmod/api/inc"
	"${SFE}/src/external/glm"
	"${SFE}/src/external/boost"
	"../external/bullet/src"
	"${SFE}/src/external/raknet/Source"
	"../external/glew/include"
	"../external/glfw/include"
	"../external/cegui/cegui/include"
	)

# Linker Options
target_link_libraries(${PROJECT_NAME}
	glew
	glfw
	boost
	bullet
	raknet
	fmod
	CEGUI
	png
	z
	squirrel
	)
