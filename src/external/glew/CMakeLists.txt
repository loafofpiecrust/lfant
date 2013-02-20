project(glew)

set(INCLUDE_DIR "include")
set(SOURCE_DIR "src")

# Source Dir
file(GLOB_RECURSE SRC_FILES "${SOURCE_DIR}/*.c")
file(GLOB_RECURSE HEADER_FILES "${INCLUDE_DIR}/*.h")

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Includes
include_directories("${INCLUDE_DIR}")

add_definitions(
	-DGLEW_BUILD_DLL
	)

target_link_libraries(${PROJECT_NAME}
	GL
	GLU
	pthread
	X11
	)
