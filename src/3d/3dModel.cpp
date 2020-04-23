#include "3dModel.hpp"
#include "io/FileSystem.hpp"
#include "private/cgltf/cgltf.h"
#include <sokol_gfx.h>
#include "gfx/gfx.hpp"
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
				p_world->AddDerivedComponent<MeshNode, Node3D>(entity, CastComponentHandle<Node3D, MeshNode>(node));
			}
			else if (node.Component->GetId() == Camera::Id)
			{
				p_world->AddDerivedComponent<Camera, Node3D>(entity, CastComponentHandle<Node3D, Camera>(node));
			}
			else
			{
				log_error("Unknown component Id %d", node.Component->GetId());
			}
		}
	}

} // namespace ari::en
