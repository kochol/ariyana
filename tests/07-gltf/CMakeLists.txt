fips_begin_app(t07gltf cmdline)
    fips_vs_warning_level(3)
    fips_deps(ari_3d ari_fs_local)
    fips_files(
        gltf.cpp
	)
	ari_tex(assets/CesiumLogoFlat.png ${texfmt})
	ari_tex(assets/DuckCM.png ${texfmt})
    ari_assets()    
fips_end_app()

ari_proj(t07gltf)