#pragma once
#include "en/System.hpp"
#include "en/EventSubscriber.hpp"
#include "FrameData.hpp"
#include "core/containers/StaticArray.hpp"
#include "sx/math.h"

namespace ari::en
{
	class Camera;
	class BoxShape;
	class Node3D;

	class SceneSystem: 
		public System,
		public EventSubscriber<events::OnComponentAssigned<Camera>>,
		public EventSubscriber<events::OnComponentRemoved<Camera>>,
		public EventSubscriber<events::OnComponentAssigned<BoxShape>>,
		public EventSubscriber<events::OnComponentRemoved<BoxShape>>
	{
	public:

		//! Constructor
		SceneSystem();

		//! Destructor
		~SceneSystem();

		void Update(World* _world, const float& _elapsed, UpdateState::Enum _state) override;
		void Configure(World* p_world) override;
		void Unconfigure(World* _world) override;
		bool NeedUpdateOn(UpdateState::Enum _state) override;

		void Receive(World* world, const events::OnComponentAssigned<Camera>& event) override;
		void Receive(World* world, const events::OnComponentRemoved<Camera>& event) override;
		void Receive(World* world, const events::OnComponentAssigned<BoxShape>& event) override;
		void Receive(World* world, const events::OnComponentRemoved<BoxShape>& event) override;

	protected:

		Camera		*	m_pActiveCamera;
		core::StaticArray<FrameData, 3>	
						m_aFrameData;
		int				m_FameDataTurnIndex = 0;
		FrameData	*	m_pFrameDataUnused,			// This is the unused frame data pointers
					*	m_pFrameDataTransforms,		// This is the transform calculated nodes
					*	m_pFrameDataVisible;		// This is the visible nodes that must be rendered.

		void CalcTransform(Node3D* node, sx_mat4* parentMat);

	}; // SceneSystem
	
} // ari::en
