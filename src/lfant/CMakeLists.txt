project(lfant)

# Versioning
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

set(INCLUDE_DIR "${SFE}/src")
set(SOURCE_DIR ".")

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	# System
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Source Info
file(GLOB_RECURSE SRC_FILES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${INCLUDE_DIR}/lfant/*.h")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g")

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
#add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# Dependencies
add_dependencies(${PROJECT_NAME} ${SFE}/external)

# Includes Info
include_directories(
	"${INCLUDE_DIR}"
	"${SFE}/external/fmod/api/inc"
	"${SFE}/external/glm"
	"${SFE}/external/boost"
	"${SFE}/external/bullet/src"
	"${SFE}/external/raknet/Source"
	"${SFE}/external/glew/include"
	"${SFE}/external/glfw/include"
	"${SFE}/external/rocket/Include"
	"${SFE}/external/angelscript/include"
	"${SFE}/external/angelscript/add_on"
#	"${SFE}/external/json_spirit/include"
	)

# Linker Options
target_link_libraries(${PROJECT_NAME}
	glew
	glfw
	boost
	bullet
	fmod
	rocket
	:libpng.a
	z
	angelscript
	yaml
#	jsonspirit
	)
