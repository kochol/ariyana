#include "gfx.hpp"
#include "sokol_gfx.h"
#include "core/containers/Array.hpp"
#include "io/FileSystem.hpp"
#include "private/tinyktx.h"
#include "core/log.h"
#include "core/string/StringBuilder.hpp"

// Include shaders
#include "basic.glsl.h"

namespace ari
{
    namespace gfx
    {
		core::Array<sg_bindings> g_binds_array;

		static sx_mat4 g_mView, g_mProj, g_mViewProj;

		ShaderHanlde g_shaders[int(ShaderType::Count)];

		void SetupShaders()
		{
			g_shaders[int(ShaderType::Basic)] = CreateShader(ari_box_shader_desc());
		}

		ShaderHanlde GetShader(ShaderType shader)
		{
			return g_shaders[int(shader)];
		}

		//------------------------------------------------------------------------------
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

		//------------------------------------------------------------------------------
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

		//------------------------------------------------------------------------------
		void DestroyBuffer(BufferHandle& buffer)
		{
			sg_destroy_buffer({ buffer.Index });
			core::HandleManager<BufferHandle>::RemoveHandle(buffer.Handle);
			buffer.Handle = buffer.Index = core::aInvalidHandle;
		}

		//------------------------------------------------------------------------------
		ShaderHanlde CreateShader(const sg_shader_desc* desc)
		{
			const sg_shader shader = sg_make_shader(desc);
			return { core::HandleManager<ShaderHanlde>::CreateHandleByIndex(shader.id), shader.id };
		}

		//------------------------------------------------------------------------------
		void DestroyShader(ShaderHanlde& shader)
		{
			sg_destroy_shader({ shader.Index });
			core::HandleManager<ShaderHanlde>::RemoveHandle(shader.Handle);
			shader.Handle = shader.Index = core::aInvalidHandle;
		}

		//------------------------------------------------------------------------------
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
			desc.index_type = (sg_index_type)setup.index_type;
			desc.rasterizer.cull_mode = SG_CULLMODE_BACK;
			const sg_pipeline pipeline = sg_make_pipeline(&desc);
			return { core::HandleManager<PipelineHandle>::CreateHandleByIndex(pipeline.id), pipeline.id };
		}

		//------------------------------------------------------------------------------
		void DestroyPipeline(PipelineHandle& pipeline)
		{
			sg_destroy_pipeline({ pipeline.Index });
			core::HandleManager<PipelineHandle>::RemoveHandle(pipeline.Handle);
			pipeline.Handle = pipeline.Index = core::aInvalidHandle;
		}

		//------------------------------------------------------------------------------
		void ApplyPipeline(const PipelineHandle& pipeline)
		{
			sg_apply_pipeline({ pipeline.Index });
		}

		//------------------------------------------------------------------------------
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

		//------------------------------------------------------------------------------
		void DestroyBinding(BindingHandle& binding)
		{
			core::HandleManager<BindingHandle>::RemoveHandle(binding.Handle);
			binding.Handle = binding.Index = core::aInvalidHandle;
		}

		//------------------------------------------------------------------------------
		void ApplyBindings(const BindingHandle& handle)
		{
			sg_apply_bindings(&g_binds_array[handle.Index]);
		}

		//------------------------------------------------------------------------------
		void SetTexture(const BindingHandle& handle, int stage, TextureHandle texture)
		{
			g_binds_array[handle.Index].fs_images[stage].id = texture.Index;
		}

		//------------------------------------------------------------------------------
		void ApplyUniforms(ShaderStage _stage, int _ub_index, const void* _data, int _size)
		{
			sg_apply_uniforms((sg_shader_stage)_stage, _ub_index, _data, _size);
		}

		//------------------------------------------------------------------------------
		void BeginDefaultPass(const io::WindowHandle& _handle)
		{
			sg_pass_action pass;
			core::Memory::Fill(&pass, sizeof(sg_pass_action), 0);
			const core::RectI r = io::GetWindowSize(_handle);
			sg_begin_default_pass(&pass, r.width, r.height);
		}

		//------------------------------------------------------------------------------
		void EndPass()
		{
			sg_end_pass();
		}

		//------------------------------------------------------------------------------
		void Commit()
		{
			sg_commit();
		}

		//------------------------------------------------------------------------------
		void Draw(int base_element, int num_elements, int num_instances)
		{
			sg_draw(base_element, num_elements, num_instances);
		}

		//------------------------------------------------------------------------------
		void SetViewMatrix(const sx_mat4& _view)
		{
			g_mView = _view;
			g_mViewProj = g_mProj * g_mView;
		}

		//------------------------------------------------------------------------------
		sx_mat4 GetViewMatrix()
		{
			return g_mView;
		}

		//------------------------------------------------------------------------------
		void SetProjMatrix(const sx_mat4& _proj)
		{
			g_mProj = _proj;
			g_mViewProj = g_mProj * g_mView;
		}

		//------------------------------------------------------------------------------
		sx_mat4 GetProjMatrix()
		{
			return g_mProj;
		}

		//------------------------------------------------------------------------------
		void SetViewProjMatrix(const sx_mat4& _view, const sx_mat4& _proj)
		{
			g_mView = _view;
			g_mProj = _proj;
			g_mViewProj = g_mProj * g_mView;
		}

		//------------------------------------------------------------------------------
		sx_mat4 GetViewProjMatrix()
		{
			return g_mViewProj;
		}

		//------------------------------------------------------------------------------
		static void tinyktxCallbackError(void* user, char const* msg) {
			log_error("Tiny_Ktx ERROR: %s", msg);
		}

		//------------------------------------------------------------------------------
		static void* tinyktxCallbackAlloc(void* user, size_t size) {
			return core::Memory::Alloc(int(size));
		}

		//------------------------------------------------------------------------------
		static void tinyktxCallbackFree(void* user, void* data) {
			core::Memory::Free(data);
		}

		//------------------------------------------------------------------------------
		static size_t tinyktxCallbackRead(void* user, void* data, size_t size) {
			auto buffer = reinterpret_cast<core::Buffer*>(user);
			return buffer->Read(data, int(size));
		}

		//------------------------------------------------------------------------------
		static bool tinyktxCallbackSeek(void* user, int64_t offset) {
			auto buffer = reinterpret_cast<core::Buffer*>(user);
			return buffer->Seek(int(offset));
		}

		//------------------------------------------------------------------------------
		static int64_t tinyktxCallbackTell(void* user) {
			const auto buffer = reinterpret_cast<core::Buffer*>(user);
			return buffer->Tell();
		}

		//------------------------------------------------------------------------------
		sg_pixel_format GetPixelFormatFromTinyktxFormat(TinyKtx_Format format)
		{
			switch (format)
			{
			case TKTX_BC1_RGBA_UNORM_BLOCK:
				return SG_PIXELFORMAT_BC1_RGBA;
			case TKTX_BC2_UNORM_BLOCK:
				return SG_PIXELFORMAT_BC2_RGBA;
			case TKTX_BC3_UNORM_BLOCK:
				return SG_PIXELFORMAT_BC3_RGBA;
			case TKTX_ETC2_R8G8B8_UNORM_BLOCK:
				return SG_PIXELFORMAT_ETC2_RGB8;
			default :
				log_warn("Unsupported texture format %d", format);
				return SG_PIXELFORMAT_NONE;
			}
		}

		//------------------------------------------------------------------------------
		TextureHandle CreateTexture(core::String _path)
		{
			sg_image img = sg_alloc_image();

			core::StringBuilder str = _path;
			str.Append(".ktx");
			io::LoadFile(str.GetString(), [img](core::Buffer* buffer)
			{
				TinyKtx_Callbacks callbacks{
						&tinyktxCallbackError,
						&tinyktxCallbackAlloc,
						&tinyktxCallbackFree,
						&tinyktxCallbackRead,
						&tinyktxCallbackSeek,
						&tinyktxCallbackTell
					};
				// Load with tinyktx
				auto ctx = TinyKtx_CreateContext(&callbacks, buffer);
				if (!TinyKtx_ReadHeader(ctx))
				{
					log_error("Can't load the texture. Not a valid KTX format.");
					TinyKtx_DestroyContext(ctx);
					sg_fail_image(img);
					return;
				}
				uint32_t w = TinyKtx_Width(ctx);
				uint32_t h = TinyKtx_Height(ctx);
				uint32_t d = TinyKtx_Depth(ctx);
				uint32_t s = TinyKtx_ArraySlices(ctx);
				sg_pixel_format fmt = GetPixelFormatFromTinyktxFormat(TinyKtx_GetFormat(ctx));
				if (fmt == SG_PIXELFORMAT_NONE) 
				{
					log_error("Can't load the texture. Unknown format.");
					TinyKtx_DestroyContext(ctx);
					sg_fail_image(img);
					return;
				}


				sg_image_desc desc;
				core::Memory::Fill(&desc, sizeof(sg_image_desc), 0);
				desc.type = SG_IMAGETYPE_2D;
				desc.render_target = false;
				desc.width = w;
				desc.height = h;
				desc.depth = d;
				desc.num_mipmaps = int(TinyKtx_NumberOfMipmaps(ctx));
				desc.pixel_format = fmt;
				desc.min_filter = SG_FILTER_LINEAR;
				desc.mag_filter = SG_FILTER_LINEAR;

				for (auto i = 0u; i < TinyKtx_NumberOfMipmaps(ctx); ++i) {
					desc.content.subimage[0][i].size = TinyKtx_ImageSize(ctx, i);
					void* pixels = core::Memory::Alloc(desc.content.subimage[0][i].size);
					desc.content.subimage[0][i].ptr = pixels;

					core::Memory::Copy(TinyKtx_ImageRawData(ctx, i), pixels, desc.content.subimage[0][i].size);
					if (w > 1) w = w / 2;
					if (h > 1) h = h / 2;
					if (d > 1) d = d / 2;
				}

				TinyKtx_DestroyContext(ctx);
				
				sg_init_image(img, &desc);
			});

			return { core::HandleManager<TextureHandle>::CreateHandleByIndex(img.id), img.id };
		}

    } // namespace gfx
    
} // namespace ari
