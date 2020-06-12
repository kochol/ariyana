#pragma once

#include "Gui.hpp"
#include "core/Delegate.hpp"
#include "en/World.hpp"

namespace ari
{
	namespace gui
	{
		class Button : public Gui
		{
		public:

			ARI_COMPONENT_CHILD(Button, Gui)

			bool BeginRender() override;

			DelegateNoParam<void> OnClick;

			char* Label;
		};

	}

} // ari
