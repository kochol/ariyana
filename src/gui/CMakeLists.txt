fips_begin_module(ari_gui)
    set(CMAKE_CXX_STANDARD 17)
	ari_setup()
	fips_vs_warning_level(3)
	fips_deps(ari_en cimgui)
    fips_files(
			Button.cpp		Button.hpp
			CheckBox.cpp	CheckBox.hpp		
			Gui.hpp
			GuiSystem.cpp	GuiSystem.hpp
			Image.cpp			Image.hpp
			Label.cpp			Label.hpp
			Popup.cpp			Popup.hpp
			TextBox.cpp		TextBox.hpp
			Window.cpp		Window.hpp
    )
    
    if (ARI_GLFW)
        add_definitions(-DARI_GLFW)
    endif()

fips_end_module()
