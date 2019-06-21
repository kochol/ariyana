#
# project: ari
#
cmake_minimum_required(VERSION 2.8)

# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

option(ARI_BUILD_TESTS "Build engine test projects" ON)
option(ARI_OPENGL "Use OpenGL as rendering backend" ON)

fips_setup(PROJECT ari)
	include_directories(src)
    fips_add_subdirectory(src)

    if (ARI_BUILD_TESTS)
        fips_ide_group(Tests)
        fips_add_subdirectory(tests)
    endif()
fips_finish()
