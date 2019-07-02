#include "Window.hpp"

namespace ari
{
	namespace gui
	{
		Window::Window() : Name(nullptr), CloseButton(false), isOpen(true), Flags(0)
		{
		}

		bool Window::BeginRender()
		{
			ImGui::SetNextWindowPos(
				Pos
				, ImGuiCond_FirstUseEver
			);
			ImGui::SetNextWindowSize(
				Size
				, ImGuiCond_FirstUseEver
			);
			if (CloseButton)
				return ImGui::Begin(Name
					, &isOpen
					, Flags
				);
			return ImGui::Begin(Name
				, nullptr
				, Flags
			);
		}

		void Window::EndRender()
		{
			ImGui::End();
		}

	}

} // ari
