#include "RenderSystem.hpp"
#include "gfx/Viewport.hpp"
#include "BoxShape.hpp"
#include "FrameData.hpp"
#include "Camera.hpp"
#include "en/World.hpp"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Node3D)

	RenderSystem::RenderSystem(): m_pFrameDataCurrent(nullptr), m_pFrameDataNext(nullptr)
	{
		TargetWindow.Handle = 0;
		TargetWindow.Index = 0;
	}

	RenderSystem::~RenderSystem()
	{
		BoxShape::Shutdown();
	}

	void RenderSystem::Update(World* _world, const float& _elapsed, UpdateState::Enum _state)
	{
		ari::gfx::RenderToWindow(TargetWindow);

		gfx::BeginDefaultPass();

		m_pFrameDataCurrent = m_pFrameDataNext;
		if (m_pFrameDataCurrent)
		{
			// Set the camera
			if (m_pFrameDataCurrent->Camera)
			{
				// TODO: Set viewport
				gfx::Viewport* pViewport = m_pFrameDataCurrent->Camera->GetViewport();
				gfx::SetViewProjMatrix(m_pFrameDataCurrent->Camera->_view, m_pFrameDataCurrent->Camera->_proj);
			}
			for (auto node : m_pFrameDataCurrent->Nodes)
			{
				node->Render(m_pFrameDataCurrent->FrameDataTurnIndex);
			}
		}

		gfx::EndPass();
		gfx::Commit();
		ari::gfx::Present(TargetWindow);

	} // Update

	void RenderSystem::Configure(World * p_world)
	{
		p_world->Subscribe<events::OnComponentAssigned<BoxShape>>(this);
		p_world->Subscribe<events::OnFrameData>(this);

	} // Configure

	void RenderSystem::Unconfigure(World * p_world)
	{
		p_world->unsubscribeAll(this);

	} // Unconfigure

	bool RenderSystem::NeedUpdateOn(UpdateState::Enum state)
	{
		return state == UpdateState::MainThreadState;
	}

	void RenderSystem::Receive(World * world, const events::OnComponentAssigned<BoxShape>& event)
	{
		sx_unused(world);
		sx_unused(event);
		BoxShape::Init(this);
	}

	void RenderSystem::Receive(World * world, const events::OnFrameData & event)
	{	
		sx_unused(world);
		m_pFrameDataNext = event.frame_data;		
	}

} // ari
