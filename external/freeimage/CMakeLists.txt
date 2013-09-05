project(freeimage)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	# System
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Source Info
file(GLOB_RECURSE SRC_FILES
	"Source/FreeImage/*.cpp"
	"Source/FreeImageToolkit/*.cpp"
	"Source/Metadata/*.cpp"

#	"Source/LibJPEG/*.c"
#	"Source/LibPNG/*.c"
#	"Source/LibTIFF/*.c"
	"Source/LibRawLite/*.c"
	"Source/OpenEXR/*.c"
	)

#list(REMOVE_ITEM SRC_FILES
#	)

file(GLOB_RECURSE HEADER_FILES
	"Source/*.h"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})
#add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

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
	openjpeg
	png
#	exr
	)
