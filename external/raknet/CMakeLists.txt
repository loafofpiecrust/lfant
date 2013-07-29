project(raknet)

# Source Dir
file(GLOB SRC_FILES
	"Source/*.cpp"
	)

file(GLOB_RECURSE HEADER_FILES
	"Source/*.h"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
#add_executable(${PROJECT_NAME} SRC_FILES)

# Includes
include_directories(
	"Source"
	)

add_definitions(
	-DRAKNET_SUPPORT_IPV6=1
	)

#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lXext -lX11")

target_link_libraries(${PROJECT_NAME}
	pthread
	)
