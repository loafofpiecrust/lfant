project(galaga)

get_filename_component(LIBRARY_OUTPUT_PATH "./bin${ARCH}/${PLATFORM}" REALPATH)

# Subdirs
add_subdirectory(src/galaga)
add_subdirectory(src/launcher)

