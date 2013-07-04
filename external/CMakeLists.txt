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
add_subdirectory(zlib)
add_subdirectory(png)
add_subdirectory(jpeg)
add_subdirectory(freetype)
#add_subdirectory(alut)
