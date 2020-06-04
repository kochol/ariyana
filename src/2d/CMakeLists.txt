fips_begin_module(ari_2d)
	fips_vs_warning_level(3)
	fips_deps(ari_en ari_gfx)
    fips_files(
		Camera2D.hpp                Camera2D.cpp		
        Canvas.hpp                  Canvas.cpp
        FrameData2D.hpp			 
        Layout.hpp                  Layout.cpp
        Node2D.hpp
		RenderSystem2D.hpp          RenderSystem2D.cpp
        SceneSystem2D.hpp           SceneSystem2D.cpp
        Sprite.hpp                  Sprite.cpp
    )
	fips_dir(shaders)
    sokol_shader(sprite.glsl ${slang})
    
fips_end_module()