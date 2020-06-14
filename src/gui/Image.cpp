#include "Image.hpp"
#include "en/ComponentManager.hpp"

namespace ari
{
	namespace gui
	{
		ARI_COMPONENT_IMP_CHILD(Image, Gui)

		Image::Image()
		{
//			ImageUi = Oryol::IMUI::AllocImage();
		}

		Image::~Image()
		{
//			Oryol::IMUI::FreeImage(ImageUi);
		}

		bool Image::BeginRender()
		{
			ImGui::Image(ImageUi, Size);

			if (OnHovered.IsBound() && ImGui::IsItemHovered())
				OnHovered.Execute();

			return false;
		}

	} // gui

} // ari
