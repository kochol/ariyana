#pragma once
#include "core/string/String.hpp"
#include <functional>
#include "en/ComponentHandle.hpp"
#include "3d/Node3D.hpp"

namespace ari::en
{
    void LoadGltfScene(const core::String& _path, std::function<void(core::Array<ComponentHandle<Node3D>>)> OnModel);
    
} // namespace ari::en
