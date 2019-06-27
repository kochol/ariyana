#include "gfx.hpp"
#include "sokol_gfx.h"
#include "core/containers/Array.hpp"

namespace ari
{
    namespace gfx
    {
		core::Array<sg_bindings> g_binds_array;

		BufferHandle CreateVertexBuffer(int size, void* content, BufferUsage usage)
		{
			sg_buffer_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_buffer_desc), 0);
			desc.size = size;
			desc.content = content;
			desc.usage = (sg_usage)usage;
			const sg_buffer buffer = sg_make_buffer(&desc);
			return { core::HandleManager<BufferHandle>::CreateHandleByIndex(buffer.id), buffer.id };
		}

		BufferHandle CreateIndexBuffer(int size, void* content, BufferUsage usage)
		{
			sg_buffer_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_buffer_desc), 0);
			desc.size = size;
			desc.content = content;
			desc.usage = (sg_usage)usage;
			desc.type = SG_BUFFERTYPE_INDEXBUFFER;
			const sg_buffer buffer = sg_make_buffer(&desc);
			return { core::HandleManager<BufferHandle>::CreateHandleByIndex(buffer.id), buffer.id };
		}

		ShaderHanlde CreateShader(const sg_shader_desc* desc)
		{
			const sg_shader shader = sg_make_shader(desc);
			return { core::HandleManager<ShaderHanlde>::CreateHandleByIndex(shader.id), shader.id };
		}

		PipelineHandle CreatePipeline(const PipelineSetup& setup)
		{
			sg_pipeline_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_pipeline_desc), 0);
			desc.shader.id = setup.shader.Index;
			for (int i = 0; i < ARI_MAX_SHADERSTAGE_BUFFERS; ++i)
			{
				desc.layout.buffers[i].step_func = (sg_vertex_step)setup.layout.buffers[i].step;
				desc.layout.buffers[i].step_rate = setup.layout.buffers[i].stepRate;
				desc.layout.buffers[i].stride = setup.layout.buffers[i].stride;
			}
			for (int i = 0; i < ARI_MAX_VERTEX_ATTRIBUTES; ++i)
			{
				desc.layout.attrs[i].format = (sg_vertex_format)setup.layout.attrs[i].format;
				desc.layout.attrs[i].buffer_index = setup.layout.attrs[i].bufferIndex;
				desc.layout.attrs[i].offset = setup.layout.attrs[i].offset;
			}
			const sg_pipeline pipeline = sg_make_pipeline(&desc);
			return { core::HandleManager<PipelineHandle>::CreateHandleByIndex(pipeline.id), pipeline.id };
		}

		void ApplyPipeline(PipelineHandle pipeline)
		{
			sg_apply_pipeline({ pipeline.Index });
		}

		BindingHandle CreateBinding(const Bindings& bindings)
		{
			sg_bindings binds;
			core::Memory::Fill(&binds, sizeof(sg_bindings), 0);
			for (int i = 0; i < ARI_MAX_SHADERSTAGE_BUFFERS; ++i)
			{
				if (bindings.vertexBuffers[i].Handle != core::aInvalidHandle)
				{
					binds.vertex_buffers[i].id = bindings.vertexBuffers[i].Index;
					binds.vertex_buffer_offsets[i] = bindings.vertexBufferOffsets[i];
				}
			}
			if (bindings.indexBuffer.Handle != core::aInvalidHandle)
			{
				binds.index_buffer.id = bindings.indexBuffer.Index;
				binds.index_buffer_offset = bindings.indexBufferOffset;
			}
			for (int i = 0; i < ARI_MAX_SHADERSTAGE_TEXTURES; ++i)
			{
				if (bindings.vsTextures[i].Handle != core::aInvalidHandle)
					binds.vs_images[i].id = bindings.vsTextures[i].Index;
				if (bindings.fsTextures[i].Handle != core::aInvalidHandle)
					binds.fs_images[i].id = bindings.fsTextures[i].Index;
			}
			uint32_t i;
			uint32_t h = core::HandleManager<BindingHandle>::GetNewHandle(i);
			if ((int)i < g_binds_array.Size())
				g_binds_array[i] = binds;
			else
				g_binds_array.Add(binds);
			return { h , i };
		}

		void ApplyBindings(const BindingHandle& handle)
		{
			sg_apply_bindings(&g_binds_array[handle.Index]);
		}

		void BeginDefaultPass()
		{
			sg_pass_action pass;
			core::Memory::Fill(&pass, sizeof(sg_pass_action), 0);
			const core::RectI r = io::GetWindowSize({});
			sg_begin_default_pass(&pass, r.width, r.height);
		}

		void EndPass()
		{
			sg_end_pass();
		}

		void Commit()
		{
			sg_commit();
		}

		void Draw(int base_element, int num_elements, int num_instances)
		{
			sg_draw(base_element, num_elements, num_instances);
		}

    } // namespace gfx
    
} // namespace ari
