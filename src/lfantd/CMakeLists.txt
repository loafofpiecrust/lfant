
set(CMAKE_D_USE_PHOBOS true)

project(lfant D)
SET(CMAKE_D_SOURCE_FILE_EXTENSIONS d)

file(GLOB_RECURSE SRC_FILES
	*.d
	${ROOT}/external/gl3n/src/*.d
	${ROOT}/external/dcollections/src/*.d
)

#add_executable(${PROJECT_NAME} ${SRC_FILES})
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

include_directories(
	..
	${ROOT}/external/unstandard
	${ROOT}/external/gl3n/src
	${ROOT}/external/dcollections/src
	${ROOT}/external/sfml/d
	${ROOT}/external/assimp/d
)

target_link_libraries(${PROJECT_NAME}
	sfml
	assimp
	bullet
	gdruntime
	gphobos2
)
