fips_begin_module(ari_2d)
	fips_vs_warning_level(3)
	fips_deps(ari_en ari_gfx)
    fips_files(
        FrameData2D.hpp			 
        Sprite.hpp              Sprite.cpp
		Camera2D.hpp              Camera2D.cpp		
        Node2D.hpp
		RenderSystem2D.hpp        RenderSystem2D.cpp
        SceneSystem2D.hpp         SceneSystem2D.cpp
    )
fips_end_module()