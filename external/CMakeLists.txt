project(sfexternal)

set(LIBRARY_OUTPUT_PATH "${ROOT}/bin${ARCH}/${PLATFORM}")

# Subdirs
add_subdirectory(glfw)
add_subdirectory(glew)
add_subdirectory(boost)
add_subdirectory(bullet)
add_subdirectory(assimp)
#add_subdirectory(squirrel)
add_subdirectory(angelscript)
add_subdirectory(cegui)
#add_subdirectory(devil)
#add_subdirectory(wxwidgets)
