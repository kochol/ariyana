#pragma once

#include "en/System.hpp"
#include "en/EventSubscriber.hpp"
#include "FrameData2D.hpp"
#include "core/containers/StaticArray.hpp"
#include "sx/math.h"
#include "io/Window.hpp"

namespace ari::en
{
	class Camera2D;
	class Sprite;
	class Node2D;

	class SceneSystem2D :
		public System,
		public EventSubscriber<events::OnComponentAssigned<Camera2D>>,
		public EventSubscriber<events::OnComponentRemoved<Camera2D>>
	{
	public:
		
		//! Constructor
		SceneSystem2D();

		//! Destructor
		~SceneSystem2D();

		void Update(World* p_world, const float& _elapsed, UpdateState::Enum _state) override;
		void Configure(World* p_world) override;
		void Unconfigure(World* p_world) override;
		bool NeedUpdateOn(UpdateState::Enum _state) override;

		void Receive(World* p_world, const events::OnComponentAssigned<Camera2D>& event) override;
		void Receive(World* p_world, const events::OnComponentRemoved<Camera2D>& event) override;

		io::WindowHandle		TargetWindow;

	protected:

		Camera2D		*	m_pActiveCamera2D;
		core::StaticArray<FrameData2D, 3>
			m_aFrameData2D;
		int m_FrameDataTurnIndex = 0;
		FrameData2D		*	m_pFrameDataUnused,		// This is the unused frame data pointers
						*	m_pFrameDataTransforms,	// This is the transform calculated nodes
						*	m_pFrameDataVisible;	// This is the visible nodes that must be rendered.

		void CalcTransform(Node2D* node, Node2D* parent);

	}; // SceneSystem2D

} // ari::en