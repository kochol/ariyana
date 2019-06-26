#
# project: ari
#
cmake_minimum_required(VERSION 2.8)

# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

option(ARI_BUILD_TESTS "Build engine test projects" ON)
option(ARI_OPENGL "Use OpenGL as rendering backend" ON)
option(ARI_GLFW "Use glfw on desktop" ON)

fips_setup(PROJECT ari)
    if (FIPS_EMSCRIPTEN)
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
            add_definitions(-DSOKOL_GLCORE33)
            set(slang "glsl330")
        endif()
    endif()

    set(CMAKE_CXX_STANDARD 17)

	include_directories(src)
    fips_add_subdirectory(src)

    if (ARI_BUILD_TESTS)
        fips_ide_group(Tests)
        fips_add_subdirectory(tests)
    endif()
fips_finish()
