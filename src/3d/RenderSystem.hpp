#pragma once
#include "en/System.hpp"
#include "en/EventSubscriber.hpp"

namespace ari::en
{
	class BoxShape;

	class RenderSystem: public System,
		public EventSubscriber<events::OnComponentAssigned<BoxShape>>,
		public EventSubscriber<events::OnFrameData>
	{
	public:

		RenderSystem();
		~RenderSystem();

		void Update(World* _world, const float& _elapsed, UpdateState::Enum _state) override;
		void Configure(World* p_world) override;
		void Unconfigure(World* p_world) override;
		bool NeedUpdateOn(UpdateState::Enum state) override;

		void Receive(World* world, const events::OnComponentAssigned<BoxShape>& event) override;
		void Receive(World* world, const events::OnFrameData& event) override;
	
	protected:

		FrameData			*	m_pFrameDataCurrent,
							*	m_pFrameDataNext;
	};
	
} // ari
