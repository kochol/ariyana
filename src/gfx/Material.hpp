#pragma once

#include "gfx.hpp"
#include "core/string/StringAtom.hpp"

namespace ari::gfx
{
    struct MaterialUniformInfo
    {
        core::StringAtom	Name;
        int					Size;
        int					Offset;
        ShaderStage			stage;
    };

    //! The material base class. This class dose not store the uniform params. They are in MaterialInstance
    struct Material
    {
        bool HasTexcoord = false;
        bool HasVertexColor = false;
        bool HasNormal = false;
        bool HasDirLight = false;
        bool HasPointLight = false;

        sg_shader_desc* shader_desc = nullptr;
        ShaderHandle shader;
        const core::Array<MaterialUniformInfo>* Uniforms;
        int VS_UniformSize;
        int FS_UniformSize;
    };
    
} // namespace ari::gfx
