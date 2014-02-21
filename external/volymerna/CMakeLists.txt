project(volymerna)

# Source Dir

file(GLOB SRC_FILES
	src/volymerna/*.cpp
	src/volymerna/*.h
	)

if(PLATFORM STREQUAL "linux")
	file(GLOB SRC_FILES_PLAT
		src/volymerna/linux/*.cpp
		src/volymerna/linux/*.h
	)
endif()
list(APPEND SRC_FILES "${SRC_FILES_PLAT}")

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

# Includes
include_directories(
	src
	../glm
	../boost
	../openal/include
	${ROOT}/src
	)
