#pragma once
#include "Gui.hpp"

namespace ari
{
	namespace gui
	{
		class Label : public Gui
		{
		public:

			//! Constructor
			Label();

			~Label() = default;

			bool BeginRender() override;

			const char	*	Text;
		};

	} // gui

}
