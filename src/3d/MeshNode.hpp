#pragma once

#include "Node3D.hpp"
#include "gfx/Mesh.hpp"

namespace ari::en
{
    class MeshNode : public Node3D
    {
    public:

        ARI_COMPONENT_CHILD(MeshNode, Node3D)
        
        gfx::MeshHandle Mesh;

        void Render(const int& _frameTurnIndex) override;

    };
    
} // namespace ari::en
