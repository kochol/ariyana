fips_begin_app(t09sfx cmdline)
    fips_vs_warning_level(3)
    fips_deps(ari_sfx ari_fs_local)
    fips_files(
        sfx.cpp
    )
fips_end_app()

ari_proj(t09sfx)
