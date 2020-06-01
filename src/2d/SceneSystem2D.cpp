#include "SceneSystem2D.hpp"
#include "en/World.hpp"
#include "Camera2D.hpp"
#include "Node2D.hpp"
#include "core/Rect.hpp"
#include "gfx/gfx.hpp"
#include "Canvas.hpp"

namespace ari::en
{

	SceneSystem2D::SceneSystem2D() :
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
			m_pFrameDataTransforms->Nodes.Clear();
			m_pFrameDataTransforms->Passes.Clear();
			m_pCurrentPass = nullptr;

			// Get all entities and calc transforms
			p_world->GetDerivedComponents<Node2D>([this](const ComponentHandle<Node2D>& node)
				{
					if (node.Owner->GetParent() == nullptr)
						this->CalcTransform(node.Component, nullptr);
				});

			m_FrameDataTurnIndex++;
		}
	}

	void SceneSystem2D::Configure(World * p_world)
	{
	}

	void SceneSystem2D::Unconfigure(World * p_world)
	{
	}

	bool SceneSystem2D::NeedUpdateOn(UpdateState::Enum state)
	{
#ifdef ARI_SERVER
		return false;
#else
		return state == UpdateState::SceneState;
#endif
	}

	void SceneSystem2D::CalcTransform(Entity* entity, Node2D* parent)
	{		
		entity->GetComponents<Node2D>([parent, this](const ComponentHandle<Node2D>& n)
			{
				CalcTransform(n.Component, parent);
			});
		auto l = entity->GetChildren(Entity::Id);
		for (auto e: l)
		{
			CalcTransform(reinterpret_cast<Entity*>(e), parent);
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

		if (node->GetId() == Canvas::Id)
		{
			// We have a new canvas create a render pass
			a_assert(m_pCurrentPass == nullptr); // Can not add one canvas inside another one
			m_pFrameDataTransforms->Passes.Add(PassData2D());
			m_pCurrentPass = &m_pFrameDataTransforms->Passes[m_pFrameDataTransforms->Passes.Size() - 1];
			m_pCurrentPass->canvas = reinterpret_cast<Canvas*>(node);
		}

		if (node->_isRenderable)
		{
			// Add it to frame data
			if (m_pCurrentPass)
			{
				if (node->GetId() == Camera2D::Id)
					m_pCurrentPass->Nodes.Insert(0, node);
				else
					m_pCurrentPass->Nodes.Add(node);
			}
			else
			{
				if (node->GetId() == Camera2D::Id)
					m_pFrameDataTransforms->Nodes.Insert(0, node);
				else
					m_pFrameDataTransforms->Nodes.Add(node);
			}
		}
		node->GetChildren([parent, this](Node* n)
			{
				if (n->GetBaseId() == Node2D::Id)
				{
					CalcTransform(reinterpret_cast<Node2D*>(n), parent);
				}
				else if (n->GetId() == Entity::Id)
				{
					CalcTransform(reinterpret_cast<Entity*>(n), parent);
				}
			});

		if (node->GetId() == Canvas::Id)
			m_pCurrentPass = nullptr;

	} // CalcTransform

} // ari::en