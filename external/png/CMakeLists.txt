project(png)

file(GLOB SRC_FILES
	"*.c"
	)

file(GLOB HEADER_FILES
	"*.h"
	)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

include_directories(
	"."
	)

target_link_libraries(${PROJECT_NAME}
	z
	m
	)