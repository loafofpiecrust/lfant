
# More source files
file(GLOB TEMP
	*.cpp
)

list(APPEND SRC_FILES "${TEMP}")
set(SRC_FILES "${SRC_FILES}" PARENT_SCOPE)


# libs to link
list(APPEND LIBS
	gwen
)
set(LIBS "${LIBS}" PARENT_SCOPE)


# dirs to include
list(APPEND INCLUDES
	${ROOT}/external/gwen/include
	${ROOT}/external/sfml/include
)
set(INCLUDES "${INCLUDES}" PARENT_SCOPE)