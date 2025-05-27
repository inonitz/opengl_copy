project "glad"
    systemversion "latest"
    warnings      "extra"
    SpecifyGlobalProjectCXXVersion()
    -- Project Structure
    files { 
        "include/**",
        "source/**" 
    }

    -- Specify Include Headers
    includedirs {
        "include",
    }

    -- Build Directories &// Structure
    SetupBuildDirectoriesForLibrary()

    -- Build Options
    buildoptions {
        "-march=native"
    }

    -- Linking Options
    LinkToStandardLibraries()

    
    -- Macros
    filter { "system:windows" }
        defines { "SYSTEM_WINDOWS" , "_CRT_SECURE_NO_WARNINGS" }
    filter {}
    filter { "configurations:*Lib" }
        defines { }
    filter { "configurations:*Dll" }
        defines { "GLAD_GLAPI_EXPORT", "GLAD_GLAPI_EXPORT_BUILD" }
    filter {}
    defines {}

    -- Custom Pre &// Post build Actions
