macro(ari_tex tex fmt)
    set(args "{ texop: '${fmt}' }")
    fips_generate(TYPE AriTex FROM ${tex} HEADER ${tex}.h OUT_OF_SOURCE ARGS ${args})
endmacro()

macro(ari_proj target)
    if (FIPS_MSVC)
        # set the deploy-directory as the debugger working directory
        set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    endif()
endmacro()

macro(ari_assets)
    if (FIPS_ANDROID)
        fips_generate(TYPE Assets FROM assets HEADER assets.h SOURCE assets.cpp OUT_OF_SOURCE ARGS "{ deploy_dir: \"${FIPS_PROJECT_BUILD_DIR}\", cur_dir: \"${CMAKE_CURRENT_LIST_DIR}\", app_name: \"${CurTargetName}\" }")
    else()
        fips_generate(TYPE Assets FROM assets HEADER assets.h SOURCE assets.cpp OUT_OF_SOURCE ARGS "{ deploy_dir: \"${FIPS_PROJECT_DEPLOY_DIR}\", cur_dir: \"${CMAKE_CURRENT_LIST_DIR}\" }")
    endif()
endmacro()

macro(ari_setup)
    set(texfmt "-q 128")
    if (ARI_SERVER)
        add_definitions(-DARI_SERVER)
        add_definitions(-DSOKOL_DUMMY_BACKEND)
        set(slang "glsl330")
    elseif (FIPS_EMSCRIPTEN)
        add_definitions(-DSOKOL_GLES3)
        set(slang "glsl300es:glsl100")
    elseif (FIPS_RASPBERRYPI)
        add_definitions(-DSOKOL_GLES2)
        set(slang "glsl100")
    elseif (FIPS_ANDROID)
        add_definitions(-DSOKOL_GLES3)
        set(slang "glsl300es")
    elseif (SOKOL_USE_D3D11)
        add_definitions(-DSOKOL_D3D11)
        set(slang "hlsl5")
    elseif (SOKOL_USE_METAL)
        add_definitions(-DSOKOL_METAL)
        if (FIPS_IOS)
            set(slang "metal_ios:metal_sim")
        else()
            set(slang "metal_macos")
        endif()
    else()
        if (FIPS_IOS)
            add_definitions(-DSOKOL_GLES3)
            set(slang "glsl300es")
        else()
            set(ARI_BUILD_TESTS ON)
            set(slang "glsl330")
            add_definitions(-DSOKOL_GLCORE33)
        endif()
    endif()

    # POSIX platform defines
    if (FIPS_POSIX)
        add_definitions(-DARI_POSIX=1)
        if (FIPS_LINUX)
            add_definitions(-DARI_LINUX=1)
        endif()
        if (FIPS_OSX)
            add_definitions(-DARI_OSX=1)
            if (FIPS_MACOS)
                add_definitions(-DARI_MACOS=1)
            endif()
            if (FIPS_IOS)
                add_definitions(-DARI_IOS=1)
            endif()
        endif()
        if (FIPS_EMSCRIPTEN)
            add_definitions(-DARI_EMSCRIPTEN=1)
        endif()
        if (FIPS_ANDROID)
            add_definitions(-DARI_ANDROID=1)
        endif()
    endif()

    # Windows platform defines
    if (FIPS_WINDOWS)
        add_definitions(-DARI_WINDOWS=1)
        if (FIPS_WIN32)
            add_definitions(-DARI_WIN32=1)
        endif()
        if (FIPS_WIN64)
            add_definitions(-DARI_WIN64=1)
        endif()
    endif()

endmacro()