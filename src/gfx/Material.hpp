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
        ShaderStage			Stage;
        bool                SystemData; //! This uniform is filled by engine or can be set by user
    };

    //! The material base class. This class dose not store the uniform params. They are in MaterialInstance
    struct Material
    {
        bool HasTexcoord = false;
        bool HasVertexColor = false;
        bool HasNormal = false;
        bool HasAlphaMap = false;
        bool HasShadowAoSpecularMap = false;

        // User controlled values
        float SpecularStrength = 1.0f;
        sx_vec4 BaseColor = sx_vec4f(1.f, 1.f, 1.f, 1.f);

        sg_shader_desc* shader_desc = nullptr;
        ShaderHandle shader;
        const core::Array<MaterialUniformInfo>* Uniforms;
        int VS_UniformSize;
        int FS_UniformSize;
        core::Array<float> Vs_UniformData;
        core::Array<float> Fs_UniformData;
    };
    
} // namespace ari::gfx
