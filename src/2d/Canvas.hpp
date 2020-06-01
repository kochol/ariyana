#pragma once

#include "Node2D.hpp"

namespace ari::en
{
    struct Canvas : public Node2D
    {
        ARI_COMPONENT_CHILD(Canvas, Node2D)
        
        /* data */
    };
    
} // namespace ari::en
