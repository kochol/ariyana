fips_begin_module(ari_fs_local)
    set(CMAKE_CXX_STANDARD 17)
    fips_vs_warning_level(3)
    fips_deps(ari_io)
    fips_files(
        FileSystemLocal.hpp     FileSystemLocal.cpp
    )
    add_definitions(-DARI_ENGINE_PATH="${PROJECT_SOURCE_DIR}")
fips_end_module()
