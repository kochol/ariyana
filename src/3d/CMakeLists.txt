fips_begin_module(ari_3d)
	fips_vs_warning_level(3)
	fips_deps(ari_en ari_gfx)
    fips_files(
        BoxShape.hpp            BoxShape.cpp
        Camera.hpp              Camera.cpp
        FrameData.hpp
        Node3D.hpp
        RenderSystem.hpp        RenderSystem.cpp
        SceneSystem.hpp         SceneSystem.cpp
    )
    
    fips_dir(shaders)
    sokol_shader(Box.glsl ${slang})
fips_end_module()
