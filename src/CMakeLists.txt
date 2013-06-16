#project(sfsrc)

set(LIBRARY_OUTPUT_PATH "${ROOT}/bin${ARCH}/${PLATFORM}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin${ARCH}/${PLATFORM}")

# Subdirs
#add_subdirectory(external)
add_subdirectory(lfant)
#add_subdirectory(plushi)
#add_subdirectory(editor)

