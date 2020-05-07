fips_begin_module(ari_3d)
    set(CMAKE_CXX_STANDARD 17)
	fips_vs_warning_level(3)
	fips_deps(ari_en ari_gfx)
    fips_files(
        3dModel.hpp             3dModel.cpp
        BoxShape.hpp            BoxShape.cpp
        Camera.hpp              Camera.cpp
        FrameData.hpp
        MeshNode.hpp            MeshNode.cpp
        Node3D.hpp
        RenderSystem.hpp        RenderSystem.cpp
        SceneSystem.hpp         SceneSystem.cpp
    )

    fips_dir(private)
    fips_files(
        gltf.hpp                gltf.cpp
    )

    fips_dir(private/cgltf)
    fips_files(
        cgltf.h                 cgltf.cpp
    )
    
fips_end_module()
