project(glfw)

set(INCLUDE_DIR "include")
set(SOURCE_DIR "src")

# Source Dir
aux_source_directory(${SOURCE_DIR} SRC_FILES)
file(GLOB_RECURSE HEADER_FILES "${INCLUDE_DIR}/*.h")

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Includes
include_directories("${INCLUDE_DIR}")
