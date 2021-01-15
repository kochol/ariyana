fips_begin_app(t05texcube cmdline)
    fips_vs_warning_level(3)
    fips_deps(ari_3d ari_fs_local)
    fips_files(
        texcube.cpp
    )
    set(texop "-q 190")
    ari_tex(assets/baboon.png ${texop})
    ari_assets()    
fips_end_app()

ari_proj(t05texcube)