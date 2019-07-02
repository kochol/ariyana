#pragma once
#include "Gui.hpp"

namespace ari
{
	namespace gui
	{
		class CheckBox : public Gui
		{
		public:

			CheckBox();

			~CheckBox() = default;

			bool BeginRender() override;

			bool		Checked;
			char	*	Label;
		};

	} // gui
	
} // ari
