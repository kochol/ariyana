#pragma once

#include "en/System.hpp"
#include "en/EventSubscriber.hpp"
#include "io/Window.hpp"

namespace ari::en
{
	class Sprite;

	class RenderSystem2D : public System,
		public EventSubscriber<events::OnComponentAssigned<Sprite>>,
		public EventSubscriber<events::OnFrameData>
	{
	public:

		//! Constructor
		RenderSystem2D();

		//! Destructor
		~RenderSystem2D();

		void Update(World* _world, const float& _elapsed, UpdateState::Enum _state) override;
		void Configure(World* p_world);
		void Unconfigure(World* _world) override;
		bool NeedUpdateOn(UpdateState::Enum _state) override;

		void Receive(World* world, const events::OnComponentAssigned<Sprite>& event) override;
		void Receive(World* world, const events::OnFrameData& event) override;

		io::WindowHandle		TargetWindow;

	protected:

		FrameData2D			*	m_pFrameDataCurrent,
							*	m_pFrameDataNext;
	};

} // ari::en