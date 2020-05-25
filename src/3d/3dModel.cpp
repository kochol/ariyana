#include "3dModel.hpp"
#include "io/FileSystem.hpp"
#include "gfx/Mesh.hpp"
#include "private/gltf.hpp"
#include "en/World.hpp"
#include "MeshNode.hpp"
#include "core/log.h"
#include "Camera.hpp"

namespace ari::en
{
	//------------------------------------------------------------------------------
	void Load3dScene(const core::String& _path, std::function<void(core::Array<ComponentHandle<Node3D>>)> OnModel)
	{
		LoadGltfScene(_path, std::move(OnModel));
	}

	//------------------------------------------------------------------------------
	void Add3dScene(World* p_world, const EntityHandle& entity, core::Array<ComponentHandle<Node3D>>& nodes)
	{
		for (auto node: nodes)
		{
			if (node.Component->GetId() == MeshNode::Id)
			{
				auto h = CastComponentHandle<Node3D, MeshNode>(node);
				p_world->AddDerivedComponent<MeshNode, Node3D>(entity, h);
			}
			else if (node.Component->GetId() == Camera::Id)
			{ 
				auto h = CastComponentHandle<Node3D, Camera>(node);
				p_world->AddDerivedComponent<Camera, Node3D>(entity, h);
			}
			else if (node.Component->GetId() == Node3D::Id)
			{
				p_world->AddComponent<Node3D>(entity, node);
			}	
			else
			{
				log_error("Unknown component Id %d", node.Component->GetId());
			}
		}
	}

} // namespace ari::en
