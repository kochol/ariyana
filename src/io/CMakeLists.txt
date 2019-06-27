fips_begin_module(ari_io)
    fips_vs_warning_level(3)
    fips_deps(ari_core)
    fips_files(
        Window.hpp
    )
    
    if (ARI_GLFW AND ARI_OPENGL)
        fips_dir(private/glfw)
        fips_files(
            WindowGlfw.cpp
        )
        fips_dir(private/flextgl)
        fips_files(
            flextGL.h           flextGL.c
        )
    else()

    endif()

fips_end_module()
