project "lfant"
	basedir "."
	kind "SharedLib"
	language "C++"
--	pchheader "stdafx.h"
--	pchsource "stdafx.cpp"
	files { "**.h", "**.cpp" }

	includedirs {
		ROOT.."/src",
		EXTERNAL.."/glm",
		EXTERNAL.."/assimp/include",
		EXTERNAL.."/boost",
		EXTERNAL.."/bullet/src",
		EXTERNAL.."/glew/include",
		EXTERNAL.."/glfw/include",
		EXTERNAL.."/rocket/Include",
		EXTERNAL.."/fmod/api/inc",
		EXTERNAL.."/angelscript/include",
		EXTERNAL.."/angelscript/add_on",
		EXTERNAL.."/devil/include"
	}
	
	links {
		"angelscript",
		"assimp",
		"glew",
		"glfw",
		"fmod",
		"boost",
		"bullet",
		"rocket",
		"png",
		"z",
		"rt"
	}
	
	if(os.is("linux")) then
		links {
			"pthread"
		}
	end
