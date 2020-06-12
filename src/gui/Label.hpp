#pragma once
#include "Gui.hpp"
#include "en/World.hpp"

namespace ari
{
	namespace gui
	{
		class Label : public Gui
		{
		public:

			ARI_COMPONENT_CHILD(Label, Gui)

			//! Constructor
			Label();

			~Label() = default;

			bool BeginRender() override;

			const char	*	Text;
		};

	} // gui

}
