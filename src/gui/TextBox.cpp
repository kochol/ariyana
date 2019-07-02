#include <sx/string.h>
#include "TextBox.hpp"
#include "imgui.h"

namespace ari
{
	namespace gui
	{
		TextBox::TextBox(int maxLength) : Label(nullptr), m_MaxLength(maxLength)
		{
			Text = new char[maxLength];
			core::Memory::Fill(Text, maxLength, 0);
		}

		TextBox::~TextBox()
		{
			delete[] Text;
		}

		bool TextBox::BeginRender()
		{
			ImGui::InputText(Label, Text, m_MaxLength);
			return false;
		}

		void TextBox::SetText(const char * _text) const
		{
			// copy over old content and free old buffer
            sx_strcpy(Text, m_MaxLength, _text);
		}

	} // gui

} // ari
