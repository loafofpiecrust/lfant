project "lfant"
	basedir "."
	kind "SharedLib"
	language "C++"
--	pchheader ("stdafx.h")
	files { "**.h", "**.cpp" }
	includedirs { 
		"..",
		"../external/glm",
		"../external/boost",
		"../external/bullet/src",
		"../external/glew/include",
		"../external/glfw/include",
		"../external/rocket/Include",
		"../external/fmod/api/inc",
		"../external/angelscript/include",
		"../external/angelscript/add_on"
	}
	
	links {
		"glew",
		"glfw",
		"boost",
		"bullet",
		"fmod",
		"rocket",
		"png",
		"z",
		"squirrel"
	}
