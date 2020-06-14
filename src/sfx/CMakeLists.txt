fips_begin_module(ari_sfx)
	fips_vs_warning_level(3)
	fips_deps(ari_en soloud)
    fips_files(
		AudioSource.hpp		AudioSource.cpp	
        sfx.hpp				sfx.cpp

    )
fips_end_module()