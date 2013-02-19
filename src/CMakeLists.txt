#project(sfsrc)

set(LIBRARY_OUTPUT_PATH "${SFE}/bin${ARCH}/${PLATFORM}")

# Subdirs
add_subdirectory(external)
add_subdirectory(engine)
#add_subdirectory(editor)

