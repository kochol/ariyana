#pragma once

#include "Gui.hpp"
#include "core/Delegate.hpp"

namespace ari
{
	namespace gui
	{
		class Button : public Gui
		{
		public:

			bool BeginRender() override;

			DelegateNoParam<void> OnClick;

			char* Label;
		};

	}

} // ari
