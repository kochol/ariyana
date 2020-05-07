fips_begin_app(t082d cmdline)
    fips_vs_warning_level(3)
    fips_deps(ari_2d)
    fips_files(
        2d.cpp
    )

	ari_tex(assets/baboon.png ${texfmt})
	ari_assets()

fips_end_app()