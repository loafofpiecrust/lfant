project(sfexternal)

# Subdirs
add_subdirectory(boost)
add_subdirectory(bullet)
add_subdirectory(assimp)
add_subdirectory(angelscript)
add_subdirectory(zlib)
add_subdirectory(png)
add_subdirectory(jpeg)
add_subdirectory(freetype)

if(NOT ANDROID)
add_subdirectory(glfw)
add_subdirectory(glew)
add_subdirectory(cegui)
endif()
