#pragma once
#include "Node3D.hpp"
#include "core/string/String.hpp"
#include <functional>

namespace ari::en
{
	void Load3dScene(const core::String& _path, std::function<void(core::Array<ComponentHandle<Node3D>>)> OnModel);

} // namespace ari::en
