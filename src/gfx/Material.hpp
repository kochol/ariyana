#pragma once

namespace ari::gfx
{
    //! The material base class. This class dose not store the uniform params. They are in MaterialInstance
    struct Material
    {
        bool HasTexcoord = false;
        bool HasVertexColor = false;
        bool HasNormal = false;
    };
    
} // namespace ari::gfx
