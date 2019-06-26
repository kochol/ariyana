fips_begin_module(ari_io)
    fips_vs_warning_level(3)
    fips_deps(ari_core)
    fips_files(
        Window.hpp
    )
    
    if (ARI_OPENGL)
        if (ARI_GLFW)
            fips_dir(private/glfw)
            fips_files(
                WindowGlfw.cpp
            )
            fips_dir(private/flextgl)
            fips_files(
                flextGL.h           flextGL.c
            )
        endif()
    endif()

fips_end_module()
