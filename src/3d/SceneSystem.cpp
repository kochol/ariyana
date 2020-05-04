#include "SceneSystem.hpp"
#include "en/World.hpp"
#include "Camera.hpp"
#include "Node3D.hpp"
#include "core/Rect.hpp"
#include "gfx/gfx.hpp"

namespace ari::en
{
	SceneSystem::SceneSystem(): m_pActiveCamera(nullptr), m_pFrameDataUnused(nullptr), m_pFrameDataTransforms(nullptr),
	                            m_pFrameDataVisible(nullptr)
	{
		m_aFrameData[0].FrameDataTurnIndex = 0;
		m_aFrameData[1].FrameDataTurnIndex = 1;
		m_aFrameData[2].FrameDataTurnIndex = 2;
		TargetWindow.Handle = 0;
		TargetWindow.Index = 0;
	}

	SceneSystem::~SceneSystem()
	{
	}

	void SceneSystem::Update(World* p_world, const float& _elapsed, UpdateState::Enum state)
	{
		if (state == UpdateState::SceneState)
		{
			if (m_pFrameDataTransforms)
			{
				events::OnFrameData frame_data = { m_pFrameDataTransforms };
				p_world->emit(frame_data);
			}
			if (m_FameDataTurnIndex > 2)
				m_FameDataTurnIndex = 0;
			m_pFrameDataTransforms = &m_aFrameData[m_FameDataTurnIndex];
			m_pFrameDataTransforms->FrameNumber = gfx::GetFrameNumber();
			m_pFrameDataTransforms->CameraObj = m_pActiveCamera;
			m_pFrameDataTransforms->CameraPos = m_pActiveCamera->Position;
			m_pFrameDataTransforms->Nodes.Clear();

			// Get all entities and calc transforms
			p_world->GetDerivedComponents<Node3D>(
				[this](uint32_t _entity_handle, const ComponentHandle<Node3D>& node)
				{
					this->CalcTransform(node.Component, nullptr);
				});

			if (m_pActiveCamera)
			{
				m_pActiveCamera->_view = sx_mat4_view_lookat(m_pActiveCamera->Position, m_pActiveCamera->Target, m_pActiveCamera->Up);

				core::RectI rect;
				gfx::Viewport* p = m_pActiveCamera->GetViewport();
				if (p)
					rect = p->Rect;
				else
				{
					rect = io::GetWindowSize(TargetWindow);
				}
				m_pActiveCamera->_proj = sx_mat4_perspectiveFOV(sx_torad(m_pActiveCamera->Fov),
					float(rect.width) / float(rect.height), m_pActiveCamera->zNear, m_pActiveCamera->zFar, true);				
			}

			m_FameDataTurnIndex++;
		}
	}

	void SceneSystem::Configure(World * p_world)
	{
		p_world->Subscribe<events::OnComponentAssigned<Camera>>(this);
		p_world->Subscribe<events::OnComponentRemoved<Camera>>(this);
	}

	void SceneSystem::Unconfigure(World * p_world)
	{
		p_world->unsubscribeAll(this);
	}

	bool SceneSystem::NeedUpdateOn(UpdateState::Enum state)
	{
		return state == UpdateState::SceneState;
	}

	void SceneSystem::Receive(World * world, const events::OnComponentAssigned<Camera>& event)
	{
		sx_unused(world);
		if (!m_pActiveCamera)
		{
			m_pActiveCamera = event.component;
			m_pActiveCamera->_isActive = true;
		}
	}

	void SceneSystem::Receive(World * world, const events::OnComponentRemoved<Camera>& event)
	{
		sx_unused(world);
		if (m_pActiveCamera == event.component)
		{
			m_pActiveCamera = nullptr;
		}
	}

	void SceneSystem::CalcTransform(Node3D* node, sx_mat4* parentMat)
	{
		sx_mat4 m;
		if (node->has_mat)
		{
			m = node->Transform;
		}
		else
		{
			m = sx_mat4_SRT(node->Scale.x, node->Scale.y, node->Scale.z,
				node->Rotation.x, node->Rotation.y, node->Rotation.z,
				node->Position.x, node->Position.y, node->Position.z);
		}
		if (parentMat)
			node->_finalMat[m_FameDataTurnIndex] = m * (*parentMat);
		else
			node->_finalMat[m_FameDataTurnIndex] = m;
		parentMat = &node->_finalMat[m_FameDataTurnIndex];

		if (node->_isRenderable)
		{
			// Add it to frame data
			m_pFrameDataTransforms->Nodes.Add(node);
		}
		node->GetChildren([parentMat, this](Node* n)
		{
			if (n->GetBaseId() == Node3D::Id)
			{
				CalcTransform(reinterpret_cast<Node3D*>(n), parentMat);
			}
		});
	} // CalcTransform
	
} // ari
