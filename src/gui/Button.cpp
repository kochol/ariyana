#include "Button.hpp"
#include "cimgui/imgui/imgui.h"
#include "en/ComponentManager.hpp"

namespace ari
{
	namespace gui
	{
		ARI_COMPONENT_IMP_CHILD(Button, Gui)

		bool Button::BeginRender()
		{

			if (ImGui::Button(Label) && OnClick.IsBound())
			{
				OnClick.Execute();
			}
			return false;
		}

	} // gui

} // ari
