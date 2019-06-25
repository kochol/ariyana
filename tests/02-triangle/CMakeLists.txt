fips_begin_app(02-triangle cmdline)
    fips_vs_warning_level(3)
    fips_deps(ari_gfx)
    fips_files(
        triangle.cpp
    )
    sokol_shader(triangle.glsl ${slang})
fips_end_app()