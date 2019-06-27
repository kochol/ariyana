fips_begin_module(ari_gfx)
    fips_vs_warning_level(3)
    fips_deps(ari_io)
    fips_files(
        gfx.hpp                     gfx.cpp
    )

    if (ARI_GLFW AND ARI_OPENGL)
        fips_deps(glfw3)
        fips_dir(private/glfw)
        fips_files(
            gfx_glfw.cpp
        )
    else()

    endif()

fips_end_module()
