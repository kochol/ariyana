fips_begin_module(ari_gui)
	fips_vs_warning_level(3)
	fips_deps(ari_en imgui)
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
fips_end_module()
