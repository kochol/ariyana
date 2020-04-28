#pragma once
#include "Node3D.hpp"
#include "core/string/String.hpp"
#include <functional>

namespace ari::en
{
	struct World;

	void Load3dScene(const core::String& _path, std::function<void(core::Array<ComponentHandle<Node3D>>)> OnModel);

	void Add3dScene(World* p_world, const EntityHandle& entity, core::Array<ComponentHandle<Node3D>>& nodes);

} // namespace ari::en
