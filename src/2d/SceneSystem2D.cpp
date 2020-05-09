#include "SceneSystem2D.hpp"
#include "en/World.hpp"
#include "Camera2D.hpp"
#include "Node2D.hpp"
#include "core/Rect.hpp"
#include "gfx/gfx.hpp"

namespace ari::en
{

	SceneSystem2D::SceneSystem2D() :
		m_pActiveCamera2D(nullptr),
		m_pFrameDataUnused(nullptr),
		m_pFrameDataTransforms(nullptr),
		m_pFrameDataVisible(nullptr)
	{
		m_aFrameData2D[0].FrameDataTurnIndex = 0;
		m_aFrameData2D[1].FrameDataTurnIndex = 1;
		m_aFrameData2D[2].FrameDataTurnIndex = 2;
		TargetWindow.Handle = 0;
		TargetWindow.Index = 0;
	}


	SceneSystem2D::~SceneSystem2D()
	{
	}

	void SceneSystem2D::Update(World* p_world, const float& _elapsed, UpdateState::Enum state)
	{
		if (state == UpdateState::SceneState)
		{
			if (m_pFrameDataTransforms)
			{
				events::OnFrameData2D frame_data_2d = { m_pFrameDataTransforms };
				p_world->emit(frame_data_2d);
			}
			if (m_FrameDataTurnIndex > 2)
				m_FrameDataTurnIndex = 0;
			m_pFrameDataTransforms = &m_aFrameData2D[m_FrameDataTurnIndex];
			m_pFrameDataTransforms->FrameNumber = gfx::GetFrameNumber();
			m_pFrameDataTransforms->Camera2dObj = m_pActiveCamera2D;
			m_pFrameDataTransforms->Nodes.Clear();

			// Get all entities and calc transforms
			p_world->GetDerivedComponents<Node2D>([this](uint32_t _entity_handle, const ComponentHandle<Node2D>& node)
				{
					this->CalcTransform(node.Component, nullptr);
				});
			if (m_pActiveCamera2D)
			{
				m_pActiveCamera2D->_view = sx_mat4_ident();

				core::RectI rect;
				gfx::Viewport* p = m_pActiveCamera2D->GetViewport();
				if (p)
					rect = p->Rect;
				else
				{
					rect = io::GetWindowSize(TargetWindow);
				}

				//m_pActiveCamera2D->_proj = sx_mat4_ortho_offcenter(0,0,rect.width/2, rect.height/2, 0.1f, 100, 0, true);
				m_pActiveCamera2D->_proj = sx_mat4_ortho(rect.width, rect.height, 0.1f, 100, 0, false);
			}

			m_FrameDataTurnIndex++;
		}
	}

	void SceneSystem2D::Configure(World * p_world)
	{
		p_world->Subscribe<events::OnComponentAssigned<Camera2D>>(this);
		p_world->Subscribe<events::OnComponentRemoved<Camera2D>>(this);
	}

	void SceneSystem2D::Unconfigure(World * p_world)
	{
		p_world->unsubscribeAll(this);
	}

	bool SceneSystem2D::NeedUpdateOn(UpdateState::Enum state)
	{
		return state == UpdateState::SceneState;
		
	}

	void SceneSystem2D::Receive	(World * world, const events::OnComponentAssigned<Camera2D>& event)
	{
		sx_unused(world);
		if (!m_pActiveCamera2D)
		{
			m_pActiveCamera2D = event.component;
			m_pActiveCamera2D->_isActive = true;
		}
	}

	void SceneSystem2D::Receive(World* world, const events::OnComponentRemoved<Camera2D>& event)
	{
		sx_unused(world);
		if (m_pActiveCamera2D == event.component)
		{
			m_pActiveCamera2D = nullptr;
		}
	}

	void SceneSystem2D::CalcTransform(Node2D* node, Node2D* parent)
	{
		sx_mat4 m;
		if (node->has_mat)
		{
			m = node->Transform;
		}
		else
		{
			
			m = sx_mat4_SRT(
				node->Scale.x, node->Scale.y, 1.0f, 
				0, 0, node->Rotation,
				node->Position.x, node->Position.y, 0.0f);
		}
		if (parent)
		{
			node->_finalMat[m_FrameDataTurnIndex] = parent->_finalMat[m_FrameDataTurnIndex] * m;
		}
		else
		{
			node->_finalMat[m_FrameDataTurnIndex] = m;
		}
		parent = node;

		if (node->_isRenderable)
		{
			// Add it to frame data
			m_pFrameDataTransforms->Nodes.Add(node);
		}
		node->GetChildren([parent, this](Node* n)
			{
				if (n->GetBaseId() == Node2D::Id)
				{
					CalcTransform(reinterpret_cast<Node2D*>(n), parent);
				}
			});

	} // CalcTransform

} // ari::en