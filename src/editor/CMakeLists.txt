project(editor)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	)

# Source Info
file(GLOB_RECURSE SRC_FILES
	"*.cpp"
	)

file(GLOB_RECURSE HEADER_FILES
	"*.h"
	)

# Output Library Info
#add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Dependencies
#add_dependencies(${PROJECT_NAME} ${ROOT}/external)

# Includes Info
include_directories(
	${ROOT}/src
	${ROOT}/external/glm
	${ROOT}/external/boost
	${ROOT}/external/bullet/src
	${ROOT}/external/raknet/Source
	${ROOT}/external/glew/include
	${ROOT}/external/glfw/include
	${ROOT}/external/rocket/Include
	${ROOT}/external/wxwidgets/include
	${ROOT}/external/squirrel/include
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
	png
	z
	wxwidgets
	squirrel
	)
