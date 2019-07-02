#include "CheckBox.hpp"
#include "imgui.h"

namespace ari
{
	namespace gui
	{
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
