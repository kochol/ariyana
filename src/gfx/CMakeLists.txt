fips_begin_module(ari_gfx)
    fips_vs_warning_level(3)
    fips_deps(ari_io)
    fips_files(
        gfx.hpp                     gfx.cpp
    )

    if (ARI_OPENGL)
        fips_dir(private/ogl)
        fips_files(
            gfx_ogl.hpp             gfx_ogl.cpp
        )
        if (FIPS_WINDOWS)
            fips_dir(private/ogl/win)
            fips_files(
                gfx_ogl_win.cpp
            )
        endif()
    endif()
fips_end_module()
