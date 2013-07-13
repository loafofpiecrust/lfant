project(lfant)

# Versioning
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

# Library Search Paths
link_directories(
	${LIBRARY_OUTPUT_PATH}
	# System
	/usr/lib/x86_64-linux-gnu/mesa
	)

# Source Info
file(GLOB_RECURSE SRC_FILES 
	*.cpp
	)

list(REMOVE_ITEM SRC_FILES
	${PROJECT_SOURCE_DIR}/ParticleSystem.cpp
	)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g")

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
#add_executable(${PROJECT_NAME} ${SRC_FILES})

# Dependencies
add_dependencies(${PROJECT_NAME} ${ROOT}/external)

# Includes Info
include_directories(
	../
	${ROOT}/external/png
	${ROOT}/external/jpeg
	${ROOT}/external/zlib
	${ROOT}/external/bass
	${ROOT}/external/assimp/include
	${ROOT}/external/glm
	${ROOT}/external/boost
	${ROOT}/external/bullet/src
	${ROOT}/external/glew/include
	${ROOT}/external/glfw/include
	${ROOT}/external/cegui/cegui/include
	${ROOT}/external/squirrel/include
	${ROOT}/external/devil/include
#	"${ROOT}/external/gameswf"
	)

# Linker Options
target_link_libraries(${PROJECT_NAME}
	glew
	assimp
	glfw
	boost
	bullet
	CEGUI
	png
	z
	squirrel
	jpeg
	)

if(PLATFORM STREQUAL "linux")
	target_link_libraries(${PROJECT_NAME}
		rt
		)
endif()

if(PLATFORM STREQUAL "windows")
	target_link_libraries(${PROJECT_NAME}
		winmm
		ws2_32
		wsock32
		)
endif()