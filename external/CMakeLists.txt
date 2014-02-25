project(external)


add_subdirectory(boost)
add_subdirectory(bullet)
add_subdirectory(assimp)
add_subdirectory(squirrel)
#add_subdirectory(zlib)
add_subdirectory(png)
add_subdirectory(jpeg)
add_subdirectory(tiff)
add_subdirectory(volymerna)
add_subdirectory(openal)

if(EDITOR)
add_subdirectory(wxwidgets)
endif()

if(NOT ANDROID)
add_subdirectory(glew)
add_subdirectory(glfw)
add_subdirectory(freetype)
add_subdirectory(cegui)
endif()
