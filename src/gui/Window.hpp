#pragma once
#include "Gui.hpp"
#include "imgui.h"
#include "en/World.hpp"

namespace ari
{
	namespace gui
	{
		class Window : public Gui
		{
		public:

			ARI_COMPONENT_CHILD(Window, Gui)

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
