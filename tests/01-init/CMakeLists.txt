fips_begin_app(t01init cmdline)
    fips_vs_warning_level(3)
    fips_deps(ari_gfx)
    fips_files(
        init.cpp
    )
fips_end_app()