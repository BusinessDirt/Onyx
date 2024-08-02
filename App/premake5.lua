project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {"src/**.h", "src/**.cpp"}

    includedirs {"src", "../Core/src", "../vendor/glfw/include", "%{IncludeDir.VulkanSDK}", "%{IncludeDir.spdlog}", "%{IncludeDir.stb_image}", "%{IncludeDir.glm}"}

    links {"Core"}

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    prebuildcommands { "./shaders/compile.cmd" }

    filter "configurations:Debug"
        defines {"ONYX_DEBUG"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"ONYX_RELEASE"}
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        kind "WindowedApp"
        defines {"ONYX_DIST"}
        runtime "Release"
        optimize "On"
        symbols "Off"
