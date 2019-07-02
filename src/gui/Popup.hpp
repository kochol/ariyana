#pragma once
#include "Gui.hpp"

namespace ari
{
	namespace gui
	{
		class Popup : public Gui
		{
		public:

			bool BeginRender() override;

			void EndRender() override;

			void Show();

			void Hide();

			//! Name must be unique.
			char*	Name = nullptr;

		protected:

			bool m_bDoEndPopup = false;
			bool m_bOpenPopup = false;
			bool m_bClosePopup = false;

		}; // Popup

	} // gui

} // ari
