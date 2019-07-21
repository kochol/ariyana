macro(ari_tex tex fmt)
    set(args "{ texfmt: '${fmt}' }")
    fips_generate(TYPE AriTex FROM ${tex} HEADER ${tex}.h OUT_OF_SOURCE ARGS ${args})
endmacro()
