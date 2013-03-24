project(editor)

set(INCLUDE_DIR "${SFE}/src")
set(SOURCE_DIR ".")

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	)

# Source Info
file(GLOB_RECURSE SRC_FILES
	"${SOURCE_DIR}/*.cpp"
	)

file(GLOB_RECURSE HEADER_FILES
	"${INCLUDE_DIR}/editor/*.h"
	)

# Output Library Info
#add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

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
	"../external/rocket/Include"
	"../external/wxwidgets/include"
	)

add_definitions(
	-DwxUSE_GUI
	)

# Linker Options
target_link_libraries(${PROJECT_NAME}
	lfant
	glew
	glfw
	boost
	bullet
	fmod
	rocket
	:libpng.a
	z
	squirrel
	yaml
	wxwidgets
	)
