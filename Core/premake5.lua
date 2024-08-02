project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    pchheader "stdafx.h"
    pchsource "src/stdafx.cpp"

    files { "src/**.h", "src/**.cpp" }

    includedirs {
        "src",
        "../vendor/glfw/include",
        "../vendor/stb_image",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.tinyobjloader}"
    }

    links {
        "GLFW",
        "%{Library.Vulkan}",
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines { "ONYX_DEBUG" }
        runtime "Debug"
        symbols "On"
        links {
            "%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
        }

    filter "configurations:Release"
        defines { "ONYX_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
        links {
            "%{Library.ShaderC_Release}",
            "%{Library.SPIRV_Cross_Release}",
            "%{Library.SPIRV_Cross_GLSL_Release}"
        }

    filter "configurations:Dist"
       defines { "ONYX_DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
       links {
            "%{Library.ShaderC_Release}",
            "%{Library.SPIRV_Cross_Release}",
            "%{Library.SPIRV_Cross_GLSL_Release}"
        }