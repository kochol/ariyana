#pragma once
#include "Gui.hpp"
#include "imgui.h"
#include "core/Delegate.hpp"

namespace ari
{
	namespace gui
	{
		class Image : public Gui
		{
		public:

			Image();

			~Image();

			bool BeginRender() override;

			//! Use IMUI::Bind to bind a texture to this image.
			ImTextureID					ImageUi;
			ImVec2						Size;
			DelegateNoParam<void>		OnHovered;

		}; // Image

	} // gui

} // ari
