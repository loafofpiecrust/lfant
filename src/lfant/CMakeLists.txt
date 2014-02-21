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
file(GLOB SRC_FILES
	*.cpp
	)

file(GLOB_RECURSE SRC_FILES_REC
	*.h
	net/*.cpp
	util/*.cpp
	physics/*.cpp
	platform/${PLATFORM}/*.cpp
	)
list(APPEND SRC_FILES "${SRC_FILES_REC}")

if(PLATFORM STREQUAL "android")
	list(REMOVE_ITEM SRC_FILES
		${PROJECT_SOURCE_DIR}/OpenCL.cpp
		)
	file(GLOB_RECURSE SRC_FILES_PLAT
		platform/mobile/*.cpp
	#	platform/mobile/*.c
		)
	list(APPEND SRC_FILES "${SRC_FILES_PLAT}")
else()
	file(GLOB_RECURSE SRC_FILES_PLAT
		platform/desktop/*.cpp
		)
	list(APPEND SRC_FILES "${SRC_FILES_PLAT}")
endif()

list(REMOVE_ITEM SRC_FILES
	${PROJECT_SOURCE_DIR}/ParticleSystem.cpp
	${PROJECT_SOURCE_DIR}/net/udp/*.cpp
	)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g")

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
#add_executable(${PROJECT_NAME} ${SRC_FILES})

# Dependencies
#add_dependencies(${PROJECT_NAME} ${ROOT}/external)

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
	${ROOT}/external/glfw/include
	${ROOT}/external/cegui/cegui/include
	${ROOT}/external/squirrel/include
	${ROOT}/external/devil/include
	${ROOT}/external/openal/include
	${ROOT}/external/plaidaudio
#	"${ROOT}/external/gameswf"
	)

# Linker Options
target_link_libraries(${PROJECT_NAME}
	assimp
	boost
	bullet
	png
	z
	squirrel
	jpeg
	openal
	json
	)

if(NOT ANDROID)
include_directories(
	${ROOT}/external/glew/include
	)

target_link_libraries(${PROJECT_NAME}
	glfw
	OpenCL
	CEGUI
	glew
	)

else()

include_directories(
	${ROOT}/external/glew/es/include
	)

target_link_libraries(${PROJECT_NAME}
	android
	log
	EGL
	GLESv2
	)
endif()

if(PLATFORM STREQUAL "linux")
	target_link_libraries(${PROJECT_NAME}
		rt
		)
endif()
