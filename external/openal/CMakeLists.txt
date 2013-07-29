project(openal)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

include_directories(
	"include"
	"src"
	)