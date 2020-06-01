fips_begin_module(ari_sfx)
	fips_vs_warning_level(3)
	fips_deps(ari_io soloud)
    fips_files(
        sfx.hpp         sfx.cpp
    )
fips_end_module()