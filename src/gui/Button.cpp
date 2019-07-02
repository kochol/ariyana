#include "Button.hpp"
#include "imgui.h"

namespace ari
{
	namespace gui
	{
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
