#pragma once
#include "Gui.hpp"
#include "imgui.h"

namespace ari
{
	namespace gui
	{
		class Window : public Gui
		{
		public:
			Window();

			~Window() = default;

			bool BeginRender() override;
			void EndRender() override;

			char	*	Name;
			bool		CloseButton,
						isOpen;
			ImVec2		Pos,
						Size;
			ImGuiWindowFlags	Flags;

		}; // Window

	}

} // ari
