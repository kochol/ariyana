#
# project: ari
#
if (NOT FIPS_IMPORT)
    cmake_minimum_required(VERSION 3.6)
    # include the fips main cmake file
    get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
    include("${FIPS_ROOT_DIR}/cmake/fips.cmake")
    fips_setup(PROJECT ari)
    option(ARI_BUILD_TESTS "Build engine test projects" ON)
else()
    option(ARI_BUILD_TESTS "Build engine test projects" OFF)
endif()

option(ARI_OPENGL "Use OpenGL as rendering backend" ON)
option(ARI_C99 "Create c99 API for engine" OFF)
option(ARI_SERVER "Create headless engine" OFF)

ari_setup()

set(CMAKE_CXX_STANDARD 17)

fips_include_directories(src)
fips_ide_group(Modules)
fips_add_subdirectory(src)

if (ARI_BUILD_TESTS AND NOT ARI_C99)
    fips_ide_group(Tests)
    fips_add_subdirectory(tests)
endif()

if (NOT FIPS_IMPORT)
    fips_finish()
endif()