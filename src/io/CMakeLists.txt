fips_begin_module(ari_io)
    set(CMAKE_CXX_STANDARD 17)
    ari_setup()
    fips_vs_warning_level(3)
    fips_deps(ari_core)
    fips_files(
        FileSystem.hpp          FileSystem.cpp
        FileSystemBase.hpp      FileSystemBase.cpp
        IOEvents.hpp
        IOStatus.hpp            IOStatus.cpp
        URL.hpp                 URL.cpp
        URLBuilder.hpp          URLBuilder.cpp
        Window.hpp
    )

    fips_dir(platform)
    fips_files(
        Platform.hpp            Platform.cpp
    )

    if (FIPS_WINDOWS)
        fips_files(PlatformWindows.cpp)
    elseif (FIPS_POSIX)
        fips_files(PlatformUnix.cpp)
    elseif (FIPS_ANDROID)
        fips_files(PlatformAndroid.cpp)
    endif()
    
    if (ARI_SERVER)
        fips_dir(private/server)
        fips_files(
            WindowServer.cpp
        )    
    elseif (ARI_GLFW AND ARI_OPENGL)
        fips_deps(glfw3)
        fips_dir(private/glfw)
        fips_files(
            WindowGlfw.cpp
        )
        fips_dir(private/flextgl)
        fips_files(
            flextGL.h           flextGL.c
        )
    else()
        fips_dir(private/sapp)
        fips_files(
            WindowSapp.cpp
        )
    endif()

fips_end_module()
