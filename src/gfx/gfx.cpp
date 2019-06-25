#include "gfx.hpp"
#include "sokol_gfx.h"

namespace ari
{
    namespace gfx
    {
		BufferHandle CreateVertexBuffer(int size, void* content, BufferUsage usage)
		{
			sg_buffer_desc desc;
			desc.size = size;
			desc.content = content;
			desc.usage = (sg_usage)usage;
			const sg_buffer buffer = sg_make_buffer(&desc);
			uint32_t h = core::HandleManager<BufferHandle>::GetHandleByIndex(buffer.id);
			return { h, buffer.id };
		}

		BufferHandle CreateIndexBuffer(int size, void* content, BufferUsage usage)
		{
			sg_buffer_desc desc;
			desc.size = size;
			desc.content = content;
			desc.usage = (sg_usage)usage;
			desc.type = SG_BUFFERTYPE_INDEXBUFFER;
			const sg_buffer buffer = sg_make_buffer(&desc);
			uint32_t h = core::HandleManager<BufferHandle>::GetHandleByIndex(buffer.id);
			return { h, buffer.id };
		}

		ShaderHanlde CreateShader(const sg_shader_desc* desc)
		{
			const sg_shader shader = sg_make_shader(desc);
			return { core::HandleManager<ShaderHanlde>::GetHandleByIndex(shader.id), shader.id };
		}

    } // namespace gfx
    
} // namespace ari
