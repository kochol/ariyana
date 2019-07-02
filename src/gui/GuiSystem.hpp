#pragma once

#include "en/System.hpp"
#include "Gui.hpp"

namespace ari
{
	using namespace en;

	namespace gui
	{
		class GuiSystem: public en::System
		{
		private:

		public:

			GuiSystem();

			~GuiSystem();

			void Configure(World* _world) override;

			void UnConfigure(World* _world) override;

			void Update(World* _world, float _elapsed,
				UpdateState::Enum _state) override;

			bool NeedUpdateOn(UpdateState::Enum _state) override;

		private:

			void RenderGui(Gui* _gui) const;

		};
		
	} // gui
	
} // ari
