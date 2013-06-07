project(jpeg)

file(GLOB SRC_FILES
	"*.c"
	)

file(GLOB HEADER_FILES
	"*.h"
	)

list(REMOVE_ITEM SRC_FILES
	"${PROJECT_SOURCE_DIR}/jmemmac.c"
	"${PROJECT_SOURCE_DIR}/jmemdos.c"
	"${PROJECT_SOURCE_DIR}/jmemname.c"
	"${PROJECT_SOURCE_DIR}/jmemnobs.c"
	"${PROJECT_SOURCE_DIR}/cjpeg.c"
	"${PROJECT_SOURCE_DIR}/djpeg.c"
	"${PROJECT_SOURCE_DIR}/example.c"
	"${PROJECT_SOURCE_DIR}/wrjpgcom.c"
	"${PROJECT_SOURCE_DIR}/rdjpgcom.c"
	"${PROJECT_SOURCE_DIR}/ckconfig.c"
	"${PROJECT_SOURCE_DIR}/jpegtran.c"
	)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

include_directories(
	"."
	)

target_link_libraries(${PROJECT_NAME}
	m
	)