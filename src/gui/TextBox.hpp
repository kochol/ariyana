#pragma once
#include "Gui.hpp"
#include "en/World.hpp"

namespace ari
{
	namespace gui
	{
		class TextBox : public Gui
		{
		public:

			ARI_COMPONENT_CHILD(TextBox, Gui)

			TextBox(int maxLength = 128);

			~TextBox() override;

			bool BeginRender() override;

			void SetText(const char* _text) const;

			char* Text;
			char* Label;

		private:
			int m_MaxLength;

		};

	} // gui

} // ari
