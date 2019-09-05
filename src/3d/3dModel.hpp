#pragma once
#include "Node3D.hpp"
#include "core/string/String.hpp"
#include <functional>

namespace ari::en
{
	void Load3dModel(core::String _path, std::function<void(ComponentHandle<Node3D>)> OnModel);

} // namespace ari::en
