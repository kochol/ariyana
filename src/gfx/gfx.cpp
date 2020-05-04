#include "gfx.hpp"
#include "sokol_gfx.h"
#include "core/containers/Array.hpp"
#include "io/FileSystem.hpp"
#include "private/tinyktx.h"
#include "core/log.h"
#include "core/string/StringBuilder.hpp"
#include "sx/hash.h"
#include "Material.hpp"

// Include shaders
#include "basic.glsl.h"
#include "mesh.glsl.h"

namespace ari
{
	namespace gfx
	{
		struct ShaderDescShaderHandle
		{
			const sg_shader_desc* desc = nullptr;
			ShaderHandle shader;
			uint32_t hash;
			core::Array<MaterialUniformInfo> Uniforms;
			int VS_UniformSize;
			int FS_UniformSize;

			// Shader uniform names string atoms
			core::StringAtom str_uni_mvp = "mvp",
				str_uni_matNormal = "matNormal",
				str_uni_matWorld = "matWorld",
				str_uni_camPos = "camPos",
				str_uni_specularStrength = "specularStrength",
				str_uni_lightDir = "lightDir",
				str_uni_lightColor = "lightColor",
				str_uni_lightPos = "lightPos";


			void Setup(core::StringBuilder name)
			{
				hash = sx_hash_xxh32(name.AsCStr(), name.Length(), 0);

				// Prepare uniform data
				VS_UniformSize = desc->vs.uniform_blocks[0].size;
				FS_UniformSize = desc->fs.uniform_blocks[0].size;
				int vs_offset = 0;
				int fs_offset = 0;
				if (name.Contains("mesh_"))
				{
					Uniforms.Add({ str_uni_mvp, 16, 0, ShaderStage::VertexShader, true });
					vs_offset += 16;
					int index = 5;
					while (name.Length() < index)
					{
						switch (name.At(index))
						{
						case 'T':
							// nothing to  do
							break;
						case 'V':
							if (name.Length() > index && name.At(index + 1) == 'C')
								index++;
							break;
						case 'N': // Normal
							Uniforms.Add({ str_uni_matWorld, 16, vs_offset, ShaderStage::VertexShader, true });
							Uniforms.Add({ str_uni_matNormal, 16, vs_offset + 16, ShaderStage::VertexShader, true });
							vs_offset += 32;
							Uniforms.Add({ str_uni_camPos, 3, fs_offset, ShaderStage::FragmentShader, true });
							Uniforms.Add({ str_uni_specularStrength, 1, fs_offset + 3, ShaderStage::FragmentShader, false });
							fs_offset += 4;
							break;
						case 'D': // Dir light
							Uniforms.Add({ str_uni_lightDir, 3, fs_offset, ShaderStage::FragmentShader, true });
							Uniforms.Add({ str_uni_lightColor, 3, fs_offset + 4, ShaderStage::FragmentShader, true });
							fs_offset += 8;
							break;
						case 'P': // Point light
							Uniforms.Add({ str_uni_lightPos, 3, fs_offset, ShaderStage::FragmentShader, true });
							Uniforms.Add({ str_uni_lightColor, 3, fs_offset + 4, ShaderStage::FragmentShader, true });
							fs_offset += 8;
							break;
						default:
							log_warn("Unknown shader stage %s", name.At(index));
						}
						index++;
					}
				}
			}
		};

		core::Array<sg_bindings> g_binds_array;
		core::Map<uint32_t, ShaderDescShaderHandle> MaterialShaders;

		static sx_mat4 g_mView, g_mProj, g_mViewProj, g_mWorld, g_mNormal, g_mWorldViewProj;

		ShaderHandle g_shaders[int(ShaderType::Count)];

		void SetupShaders()
		{
			g_shaders[int(ShaderType::Basic)] = CreateShader(ari_basic_shader_desc());
			g_shaders[int(ShaderType::BasicTexture)] = CreateShader(ari_basic_tex_shader_desc());
			g_shaders[int(ShaderType::BasicVertexColor)] = CreateShader(ari_basic_vertex_color_shader_desc());

			// Init material shaders
			ShaderDescShaderHandle sh;
			sh.desc = ari_mesh__shader_desc();
			sh.Setup("mesh_");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_T_shader_desc();
			sh.Setup("mesh_T");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_VC_shader_desc();
			sh.Setup("mesh_VC");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_TND_shader_desc();
			sh.Setup("mesh_TND");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_TNP_shader_desc();
			sh.Setup("mesh_TNP");
			MaterialShaders.Add(sh.hash, sh);
		}

		void SetMaterialShader(Material& material)
		{
			core::StringBuilder str("mesh_");
			if (material.HasTexcoord)
				str.Append("T");
			if (material.HasVertexColor)
				str.Append("VC");
			if (material.HasNormal)
				str.Append("N");
			if (material.HasDirLight)
				str.Append("D");
			if (material.HasPointLight)
				str.Append("P");

			uint32_t hash = sx_hash_xxh32(str.AsCStr(), str.Length(), 0);
			if (MaterialShaders.Contains(hash))
			{
				ShaderDescShaderHandle& mat = MaterialShaders[hash];
				if (!mat.shader.IsValid())
					mat.shader = CreateShader(mat.desc);
				material.shader = mat.shader;
				material.FS_UniformSize = mat.FS_UniformSize;
				material.VS_UniformSize = mat.VS_UniformSize;
				material.Uniforms = &mat.Uniforms;
				material.Fs_UniformData.Reserve(material.FS_UniformSize);
				material.Vs_UniformData.Reserve(material.VS_UniformSize);
			}
			else
			{
				log_error("Shader for material %s not found.", str.AsCStr());
			}
		}

		ShaderHandle GetShader(ShaderType shader)
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
		ShaderHandle CreateShader(const sg_shader_desc* desc)
		{
			const sg_shader shader = sg_make_shader(desc);
			return { core::HandleManager<ShaderHandle>::CreateHandleByIndex(shader.id), shader.id };
		}

		//------------------------------------------------------------------------------
		void DestroyShader(ShaderHandle& shader)
		{
			sg_destroy_shader({ shader.Index });
			core::HandleManager<ShaderHandle>::RemoveHandle(shader.Handle);
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
			desc.rasterizer.face_winding = SG_FACEWINDING_CCW;
			desc.depth_stencil.depth_write_enabled = true;
			desc.depth_stencil.depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL;
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
		void ApplyPipelineAndMaterial(const PipelineHandle& pipeline, Material* material)
		{
			SetPipelineShader(pipeline, material->shader);
			ApplyPipeline(pipeline);
			ApplyUniforms(ShaderStage::VertexShader, 0, &material->Vs_UniformData.Front(), material->VS_UniformSize);
			if (material->FS_UniformSize > 0)
				ApplyUniforms(ShaderStage::FragmentShader, 0, &material->Fs_UniformData.Front(), material->FS_UniformSize);
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

		void SetWorldMatrix(const sx_mat4& _world)
		{
			g_mWorld = _world;
			g_mWorldViewProj = g_mViewProj * g_mWorld;
		}

		sx_mat4 GetWorldMatrix()
		{
			return g_mWorld;
		}

		void SetNormalMatrix(const sx_mat4& _normal)
		{
			g_mNormal = _normal;
		}

		sx_mat4 GetNormalMatrix()
		{
			return g_mNormal;
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

		void SetWorldViewProjMatrix(const sx_mat4& _world, const sx_mat4& _view, const sx_mat4& _proj)
		{
			SetViewProjMatrix(_view, _proj);
			SetWorldMatrix(_world);
		}

		sx_mat4 GetWorldViewProjMatrix()
		{
			return g_mWorldViewProj;
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
		TextureHandle LoadTexture(core::String _path)
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
