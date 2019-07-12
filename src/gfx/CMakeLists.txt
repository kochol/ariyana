fips_begin_module(ari_gfx)
    fips_vs_warning_level(3)
    fips_deps(ari_io sx)
    fips_files(
        Application.hpp
        gfx.hpp                     gfx.cpp
        Vertices.hpp
        Viewport.hpp
    )

    fips_dir(private)
    fips_files(
        tinyktx.h                   tinyktx.cpp
    )

    if (ARI_GLFW AND ARI_OPENGL)
        fips_dir(private/glfw)
        fips_files(
            gfx_glfw.cpp
        )
    else()
        fips_dir(private/sapp)
        fips_files(
            gfx_sapp.cpp
        )
    endif()

fips_end_module()
