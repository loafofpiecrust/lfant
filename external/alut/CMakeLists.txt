project(alut)

file(GLOB_RECURSE SRC_FILES "src/*.c")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

file(GLOB_RECURSE TO_REMOVE "src/alut*.c")

list(REMOVE_ITEM SRC_FILES
	"${PROJECT_SOURCE_DIR}/src/helpers.c"
#	"${PROJECT_SOURCE_DIR}/src/alcFlanger.c"
	)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

include_directories(
	"."
	"include"
	"src"
	"src/Include"
	)

add_definitions(
#	-DHAVE_CONFIG_H=1
	)