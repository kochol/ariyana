#pragma once

#include "Node3D.hpp"
#include "gfx/Mesh.hpp"

namespace ari::en
{
    class MeshNode : public Node3D
    {
    public:

        ARI_COMPONENT_CHILD(MeshNode, Node3D)

        core::Array<gfx::MeshHandle> Meshes;

        MeshNode(/* args */);
        ~MeshNode();
    };
    
    MeshNode::MeshNode(/* args */)
    {
    }
    
    MeshNode::~MeshNode()
    {
    }
    
} // namespace ari::en
