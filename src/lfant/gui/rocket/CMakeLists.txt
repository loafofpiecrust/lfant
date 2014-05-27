
# More source files
file(GLOB TEMP
	*.cpp
)

list(APPEND SRC_FILES "${TEMP}")
set(SRC_FILES "${SRC_FILES}" PARENT_SCOPE)


# libs to link
list(APPEND LIBS
	rocket
)
set(LIBS "${LIBS}" PARENT_SCOPE)


# dirs to include
list(APPEND INCLUDES
	${ROOT}/external/rocket/Include
)
set(INCLUDES "${INCLUDES}" PARENT_SCOPE)