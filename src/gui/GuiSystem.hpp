#pragma once

#include "en/System.hpp"
#include "Gui.hpp"
#include "en/EventSubscriber.hpp"

namespace ari
{
	using namespace en;

	namespace gui
	{
		class GuiSystem: public en::System,
			public EventSubscriber<events::OnInputEvent>
		{
		private:

		public:

			GuiSystem();

			~GuiSystem();

			void Configure(World* _world) override;

			void Unconfigure(World* _world) override;

			void Update(World* _world, const float& _elapsed,
				UpdateState::Enum _state) override;

			bool NeedUpdateOn(UpdateState::Enum _state) override;

			void Receive(World* p_world, const events::OnInputEvent& event) override;

		private:

			void RenderGui(Gui* _gui) const;

		};
		
	} // gui
	
} // ari
