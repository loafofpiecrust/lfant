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
aux_source_directory(${SOURCE_DIR} SRC_FILES)
file(GLOB_RECURSE HEADER_FILES "${INCLUDE_DIR}/*.hpp")

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Dependencies
add_dependencies(${PROJECT_NAME} sfexternal)

# Includes Info
include_directories(
	"${INCLUDE_DIR}"
	"../external/fmod/api/inc"
	"../external/glm"
	"../external/boost"
	"../external/bullet/src"
	"../external/raknet/Source"
	"../external/glew/include"
	"../external/glfw/include"
	"../external/cegui/cegui/include"
	)

# Build Options

# Linker Options
target_link_libraries(${PROJECT_NAME}
	glew
	glfw
	boost
	bullet
	raknet
	fmod
	cegui
	png
	z
	squirrel
	)
