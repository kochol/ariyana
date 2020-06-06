#pragma once
#include "Gui.hpp"
#include "en/World.hpp"

namespace ari
{
	namespace gui
	{
		class CheckBox : public Gui
		{
		public:

			ARI_COMPONENT_CHILD(CheckBox, Gui)

			CheckBox();

			~CheckBox() = default;

			bool BeginRender() override;

			bool		Checked;
			char	*	Label;
		};

	} // gui
	
} // ari
