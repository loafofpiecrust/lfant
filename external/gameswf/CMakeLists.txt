project(gameswf)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

file(GLOB SRC_FILES
	"gameswf/*.cpp"
	"gameswf/gameswf_as_classes/*.cpp"
	"gameswf/plugins/file/*.cpp"
	"base/*.cpp"
#	"geometry/*.cpp"
	)

list(REMOVE_ITEM SRC_FILES
	"${PROJECT_SOURCE_DIR}/gameswf/gameswf_render_handler_d3d.cpp"
	"${PROJECT_SOURCE_DIR}/gameswf/gameswf_render_handler_ogles.cpp"
	"${PROJECT_SOURCE_DIR}/gameswf/gameswf_sound_handler_sdl.cpp"
	"${PROJECT_SOURCE_DIR}/gameswf/gameswf_test_ogl.cpp"
#	"${PROJECT_SOURCE_DIR}/gameswf/gameswf_disasm.cpp"
	"${PROJECT_SOURCE_DIR}/base/demo.cpp"
	"${PROJECT_SOURCE_DIR}/base/Stackwalker.cpp"
	"${PROJECT_SOURCE_DIR}/base/test_ogl.cpp"
	"${PROJECT_SOURCE_DIR}/base/GCBench.cpp"
	)

file(GLOB_RECURSE HEADER_FILES
	"gameswf/*.h"
	"base/*.h"
#	"geometry/*.h"
	)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

include_directories(
	"."
	"${ROOT}/external/glfw/include"
	"${ROOT}/external/jpeg"
	)

target_link_libraries(${PROJECT_NAME}
	glew
	boost
	png
	z
	rt
	jpeg
	dl
	glfw
	stdc++
	)
