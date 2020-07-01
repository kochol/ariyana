#include "Popup.hpp"
#include "cimgui/imgui/imgui.h"
#include "en/ComponentManager.hpp"

namespace ari
{
	namespace gui
	{
		ARI_COMPONENT_IMP_CHILD(Popup, Gui)

		bool Popup::BeginRender()
		{
			if (m_bOpenPopup)
			{
				ImGui::OpenPopup(Name);
				m_bOpenPopup = false;
			}
			m_bDoEndPopup = ImGui::BeginPopup(Name);
			return m_bDoEndPopup;
		}

		void Popup::EndRender()
		{
			if (m_bDoEndPopup)
			{
				if (m_bClosePopup)
				{
					ImGui::CloseCurrentPopup();
					m_bClosePopup = false;
				}
				ImGui::EndPopup();
			}
		}

		void Popup::Show()
		{
			m_bOpenPopup = true;
		}

		void Popup::Hide()
		{
			m_bClosePopup = true;
		}

	} // gui

} // ari
