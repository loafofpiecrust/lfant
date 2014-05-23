project(freeimage)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	# System
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Source Info
file(GLOB SRC_FILES
	"Source/FreeImage/*.cpp"
	"Source/FreeImageToolkit/*.cpp"
	"Source/Metadata/*.cpp"

#	"Source/LibJPEG/*.c"
#	"Source/LibPNG/*.c"
#	"Source/LibTIFF/*.c"
#	"Source/LibRawLite/*.c"
	"Source/OpenEXR/Half/half.cpp"
)

list(REMOVE_ITEM SRC_FILES
	${PROJECT_SOURCE_DIR}/Source/FreeImage/PluginRAW.cpp
	${PROJECT_SOURCE_DIR}/Source/FreeImage/PluginJP2.cpp
	${PROJECT_SOURCE_DIR}/Source/FreeImage/PluginJ2K.cpp
	${PROJECT_SOURCE_DIR}/Source/FreeImage/J2KHelper.cpp
	${PROJECT_SOURCE_DIR}/Source/FreeImage/PluginEXR.cpp
)

file(GLOB_RECURSE HEADER_FILES
	"Source/*.h"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

# Includes Info
include_directories(
	"Source"
	"Source/OpenEXR/Imath"
	"Source/OpenEXR/Iex"
	"Source/OpenEXR/IlmImf"
	"Source/OpenEXR/Half"
	)

# Linker Options
target_link_libraries(${PROJECT_NAME}
	tiff
	z
	jpeg
	png
#	exr
	)
