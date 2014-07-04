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
		EXTERNAL.."/glfw/include"
	}
	
	links {
		"assimp",
		"glew",
		"glfw",
		"boost",
		"bullet",
		"png",
		"z",
		"rt"
	}
	
	if(os.is("linux")) then
		links {
			"pthread"
		}
	end
