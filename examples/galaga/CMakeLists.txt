project(galaga)

get_filename_component(LIBRARY_OUTPUT_PATH "./bin${ARCH}/${PLATFORM}" REALPATH)

# Subdirs
add_subdirectory(src)

#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})

