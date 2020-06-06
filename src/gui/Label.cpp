#include "Label.hpp"
#include "imgui.h"
#include "en/ComponentManager.hpp"

namespace ari
{
	namespace gui
	{
		ARI_COMPONENT_IMP_CHILD(Label, Gui)
			
		Label::Label() : Text(nullptr)
		{
		}

		bool Label::BeginRender()
		{
			ImGui::Text("%s", Text);
			return true;
		}

	} // gui

} // ari
