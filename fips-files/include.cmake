macro(ari_tex tex fmt)
    set(args "{ texfmt: '${fmt}' }")
    fips_generate(TYPE AriTex FROM ${tex} HEADER ${tex}.h OUT_OF_SOURCE ARGS ${args})
endmacro()

macro(ari_proj target)
    if (FIPS_MSVC)
        # set the deploy-directory as the debugger working directory
        set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    endif()
endmacro()
