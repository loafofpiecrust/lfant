project(freetype)

file(GLOB SRC_FILES
	"src/base/ftbase.c"
	"src/base/ftinit.c"
	"src/base/ftstream.c"
	"src/base/ftdebug.c"
	"src/base/ftsystem.c"
	"src/base/ftmm.c"
	"src/bzip2/ftbzip2.c"
	"src/gzip/ftgzip.c"
	"src/bdf/bdf.c"
	"src/truetype/truetype.c"
	"src/sfnt/sfnt.c"
	"src/smooth/smooth.c"
	"src/cff/cff.c"
	"src/type1/type1.c"
	"src/type42/type42.c"
	"src/autofit/autofit.c"
	"src/cid/type1cid.c"
	"src/pfr/pfr.c"
	"src/winfonts/winfnt.c"
	"src/pcf/pcf.c"
	"src/psaux/psaux.c"
	"src/psnames/psnames.c"
	"src/pshinter/pshinter.c"
	"src/raster/raster.c"
	"src/lzw/ftlzw.c"
	)

file(GLOB HEADER_FILES
	"include/*.h"
	)

#list(REMOVE_ITEM SRC_FILES
#	)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})

include_directories(
	"include"
	)

add_definitions(
	-DFT_ERR_PREFIX=FT_Err_
	)

target_link_libraries(${PROJECT_NAME}
	m
	)