--GLOBALS
outputdir = "%{cfg.buildcfg}/%{cfg.platform}"

targetdir_clv = "Built/bin/"..outputdir.."/%{prj.name}"
objdir_clv = "Built/intermediate/"..outputdir.."/%{prj.name}"

targetdir_vendor = "Built/bin/"..outputdir.."/vendor/%{prj.name}"
objdir_vendor = "Built/intermediate/"..outputdir.."/vendor/%{prj.name}"

hlslout_dir = "../Sandbox/"
hlslout_com = hlslout_dir.."%{file.basename}"..".cso"

--Workspace Settings
workspace "Clove"
	startproject "Sandbox"

	systemversion "latest"
	cppdialect "C++17"

	configurations{
		"Debug",
		"Release"
	}

	platforms{
		"Win64-lib",
		"Win64-dll"
	}

	filter "platforms:Win64-lib"
		system "Windows"
		architecture "x64"
		staticruntime "On"

		defines{
			"CLV_STATIC=1"
		}

	filter "platforms:Win64-dll"
		system "Windows"
		architecture "x64"
		staticruntime "Off"

		defines{
			"CLV_DYNAMIC=1"
		}

	filter "system:Windows"
		defines{
			"CLV_PLATFORM_WINDOWS=1"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

		defines {
			"CLV_DEBUG=1"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

		defines {
			"CLV_RELEASE=1"
		}

--Clove Dependencies
group "Dependencies"
--IMGUI
project "ImGui"
	location "Clove/vendor/imgui"
	kind "StaticLib"
	language "C++"
		
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
        "%{prj.location}/imconfig.h",
        "%{prj.location}/imgui.h",
        "%{prj.location}/imgui.cpp",
        "%{prj.location}/imgui_draw.cpp",
        "%{prj.location}/imgui_internal.h",
        "%{prj.location}/imgui_widgets.cpp",
        "%{prj.location}/imstb_rectpack.h",
        "%{prj.location}/imstb_textedit.h",
        "%{prj.location}/imstb_truetype.h",
        "%{prj.location}/imgui_demo.cpp"
    }

	defines{
		"IMGUI_USER_CONFIG=\"../../src/Clove/ImGui/ImGuiConfig.hpp\"",
		"IMGUI_DISABLE_INCLUDE_IMCONFIG_H",
		"_CRT_SECURE_NO_WARNINGS"
	}    

--GLAD
project "Glad"
	location "Clove/vendor/Glad"
    kind "StaticLib"
    language "C"
    
	targetdir(targetdir_vendor)
	objdir(objdir_vendor)

	files{
        "%{prj.location}/include/glad/glad.h",
        "%{prj.location}/include/KHR/khrplatform.h",
        "%{prj.location}/src/glad.c"
    }

	includedirs{
		"%{prj.location}/include"
	}
    
	filter "system:Windows"    

--End: Dependencies
group ""

--CLOVE
-- Inlcude direction relative to the roof folder (solution directory)
includeDir = {}
includeDir["Glad"]	= "Clove/vendor/Glad/include"
includeDir["ImGui"] = "Clove/vendor/imgui"
includeDir["glm"]	= "Clove/vendor/glm"
includeDir["stb"]	= "Clove/vendor/stb"

project "Clove"
	location "Clove"
	language "C++"

	targetdir(targetdir_clv)
	objdir(objdir_clv)

	pchheader "clvpch.hpp"
	pchsource "Clove/src/clvpch.cpp"

	files{
		--Clove
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/res/**.glsl",
		"%{prj.name}/res/**.hlsl",

		--Non-static vendor *.cpp
		"%{prj.name}/vendor/stb/**.cpp",
		"%{prj.name}/vendor/dxerr/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",

		"%{includeDir.Glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}",
		"%{includeDir.stb}",

		"%{prj.name}/vendor/OBJ-Loader/source",

		"%{prj.name}/vendor/Event-Dispatcher",

		"%{prj.name}/vendor/dxerr"
	}

	links{
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	defines{
		"CLV_ENGINE=1",
		"CLV_EXPORT_DLL=1",
		"GLFW_INCLUDE_NONE"
	}

	postbuildcommands{
		("{COPY} "..hlslout_dir.."**.cso \"../Built/bin/"..outputdir.."/Sandbox/\"")
	}

	--hlsl shaders
	filter "files:**.hlsl"
		shadermodel "5.0"
		shaderobjectfileoutput(hlslout_com)
	filter "files:**-vs.hlsl"
		shadertype "Vertex"
	filter "files:**-ps.hlsl"
		shadertype "Pixel"

	--platforms
	filter "platforms:Win64-lib"
		kind "StaticLib"

	filter "platforms:Win64-dll"
		kind "SharedLib"

	filter "kind:SharedLib"
		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} \"../Built/bin/"..outputdir.."/Sandbox/\"")
		}

	filter "system:Windows"
		defines{
			"CLV_PLATFORM_WINDOWS=1"
		}

--SANDBOX
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir(targetdir_clv)
	objdir(objdir_clv)

	files{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"Clove/vendor/spdlog/include",

		"%{includeDir.ImGui}",
		"%{includeDir.glm}",

		"Clove/src",
		"Clove/vendor",
	}

	links{
		"Clove",
	}