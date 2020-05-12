fips_begin_sharedlib(cari)
    add_definitions(-DARI_NO_MAIN=1)
    fips_vs_warning_level(3)
    fips_deps(ari_gfx)
    fips_files(
        gfx.h                   gfx.cpp
        io.h                    io.cpp
    )
fips_end_sharedlib()