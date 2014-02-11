project(wxwidgets)

# Source Dir
file(GLOB SRC_FILES
	src/aui/*.cpp
	"src/common/*.cpp"
	"src/common/extended.c"
	"src/generic/*.cpp"
	"src/html/*.cpp"
	"src/propgrid/*.cpp"
#	"src/regex/*.c"
	"src/ribbon/*.cpp"
#	"src/richtext/*.cpp"
#	"src/stc/*.cpp"
#	"src/univ/*.cpp"
#	"src/xml/*.cpp"
	)

if(PLATFORM STREQUAL "linux")
	file(GLOB SRC_FILES_PLAT
		"src/gtk/*.cpp"
		"src/gtk/treeentry_gtk.c"
		"src/unix/*.cpp"
		)
	set(SRC_FILES ${SRC_FILES} ${SRC_FILES_PLAT})
	#list(REMOVE_ITEM SRC_FILES
	#	"${PROJECT_SOURCE_DIR}/src/ribbon/art_msw.cpp"
	#	)
endif()

if(PLATFORM STREQUAL "windows")
	file(GLOB SRC_FILES_PLAT
		"src/msw/*.cpp"
	#	"src/unix/*.cpp"
		)
	set(SRC_FILES ${SRC_FILES} ${SRC_FILES_PLAT})
endif()

list(REMOVE_ITEM SRC_FILES
	${PROJECT_SOURCE_DIR}/src/aui/tabartgtk.cpp
	${PROJECT_SOURCE_DIR}/src/common/execcmn.cpp
	${PROJECT_SOURCE_DIR}/src/common/webviewarchivehandler.cpp
#	${PROJECT_SOURCE_DIR}/src/common/prntbase.cpp
	"${PROJECT_SOURCE_DIR}/src/common/fontmgrcmn.cpp"
	"${PROJECT_SOURCE_DIR}/src/common/webview.cpp"
	"${PROJECT_SOURCE_DIR}/src/common/dummy.cpp"
#	"${PROJECT_SOURCE_DIR}/src/common/init.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/regiong.cpp"
	"${PROJECT_SOURCE_DIR}/src/gtk/webview_webkit.cpp"
#	"${PROJECT_SOURCE_DIR}/src/gtk/dc.cpp"
	"${PROJECT_SOURCE_DIR}/src/gtk/dcclient.cpp"
	"${PROJECT_SOURCE_DIR}/src/gtk/dcmemory.cpp"
	"${PROJECT_SOURCE_DIR}/src/gtk/dcscreen.cpp"
#	"${PROJECT_SOURCE_DIR}/src/unix/utilsunx.cpp"
	"${PROJECT_SOURCE_DIR}/src/unix/snglinst.cpp"
#	${PROJECT_SOURCE_DIR}/src/unix/evtloopunix.cpp
	"${PROJECT_SOURCE_DIR}/src/generic/statline.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/notebook.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/fontpickerg.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/colour.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/animateg.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/clrpickerg.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/collpaneg.cpp"
	"${PROJECT_SOURCE_DIR}/src/generic/timer.cpp"
	)

file(GLOB HEADER_FILES
	"include/wx/*.h"
	"include/wx/aui/*.h"
	)

# Library Search Paths
link_directories(
	"${LIBRARY_OUTPUT_PATH}"
	"/usr/lib/x86_64-linux-gnu/mesa"
	)

# Output Library Info
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADER_FILES})

add_definitions(
	-DwxUSE_GUI
	-DwxUSE_BASE=1
	-D__WX__
#	-D__WXGTK__
	-DSCI_LEXER
	-DLINK_LEXERS
	-DWXBUILDING
#	-DWXMAKINGDLL
	-DPIC
	-DWX_PRECOMP
	-D_FILE_OFFSET_BITS=64
	)

include_directories(
	"include"
	"${ROOT}/external/jpeg"
	"${ROOT}/external/png"
	"${ROOT}/external/zlib"
	)

#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lXext -lX11")

if(PLATFORM STREQUAL "linux")
	include_directories(
		"/usr/include/gtk-3.0"
		"/usr/include/glib-2.0"
		"/usr/lib/x86_64-linux-gnu/glib-2.0/include"
		"/usr/include/pango-1.0"
		"/usr/include/cairo"
		"/usr/include/gdk-pixbuf-2.0"
		"/usr/include/atk-1.0"
		)

	target_link_libraries(${PROJECT_NAME}
		pthread
		gtk-3
	#	gtk-x11-2.0
		gdk-3
		gdk_pixbuf-2.0
		cairo
		GL
		GLU
		X11
		Xext
		glib-2.0
		pango-1.0
		pangocairo-1.0
		gobject-2.0
		z
		png
		jpeg
		glew
		dl
		)
endif()
