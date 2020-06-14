#include "CheckBox.hpp"
#include "imgui.h"
#include "en/ComponentManager.hpp"

namespace ari
{
	namespace gui
	{
		ARI_COMPONENT_IMP_CHILD(CheckBox, Gui)

		CheckBox::CheckBox() : Checked(false), Label(nullptr)
		{
		}

		bool CheckBox::BeginRender()
		{
			ImGui::Checkbox(Label, &Checked);
			return true;
		}

	} // gui

} // ari
