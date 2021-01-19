#include "gfx.hpp"
#include "core/containers/Array.hpp"
#include "io/FileSystem.hpp"
#include "core/log.h"
#include "core/string/StringBuilder.hpp"
#include "sx/hash.h"
#include "Material.hpp"
#include "private/ktx_loader.hpp"
#include "private/basis_loader.hpp"
#include "Mesh.hpp"
#include "core/memory/ObjectPool.hpp"

#ifdef ARI_GLFW
#include "io/private/flextgl/flextGL.h"
#elif defined(ARI_ANDROID)
    #include <EGL/egl.h>
    #if defined(SOKOL_GLES3)
        #include <GLES3/gl3.h>
    #else
        #ifndef GL_EXT_PROTOTYPES
            #define GL_GLEXT_PROTOTYPES
        #endif
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
    #endif
#endif

#define SOKOL_GFX_IMPL
#include "sokol_gfx.h"

// Include shaders
#include "basic.glsl.h"
#include "mesh.glsl.h"

namespace ari
{
	namespace gfx
	{
		// Shader uniform names string atoms
		core::StringAtom str_uni_mvp = "mvp",
			str_uni_matWorld = "matWorld",
			str_uni_baseColor = "baseColor",
			str_uni_camPos = "camPos",
			str_uni_specularStrength = "specularStrength",
			str_uni_lightDir = "lightDir",
			str_uni_lightColor = "lightColor",
			str_uni_lightPos = "lightPos";

		// struct
		struct ShaderDescShaderHandle
		{
			const sg_shader_desc* desc = nullptr;
			ShaderHandle shader;
			uint32_t hash;
			core::Array<MaterialUniformInfo> Uniforms;
			int VS_UniformSize;
			int FS_UniformSize;

			void Setup(core::StringBuilder name)
			{
				hash = sx_hash_xxh32(name.AsCStr(), name.Length(), 0);
				Uniforms.Clear();

#ifdef ARI_SERVER				
				return;
#else
				// Prepare uniform data
				VS_UniformSize = desc->vs.uniform_blocks[0].size;
				FS_UniformSize = desc->fs.uniform_blocks[0].size;
				int vs_offset = 0;
				int fs_offset = 0;
				if (name.Contains("mesh_"))
				{
					Uniforms.Add({ str_uni_mvp, 16, 0, ShaderStage::VertexShader, true });
					vs_offset = 16;
					Uniforms.Add({ str_uni_baseColor, 4, 0, ShaderStage::FragmentShader, false });
					fs_offset = 4;
					int index = 5;
					while (name.Length() > index)
					{
						switch (name.At(index))
						{
						case 'A':
						case 'T':
						case 'M':
							// nothing to  do
							break;
						case 'V':
							if (name.Length() > index && name.At(index + 1) == 'C')
								index++;
							break;
						case 'N': // Normal
							Uniforms.Add({ str_uni_matWorld, 16, vs_offset, ShaderStage::VertexShader, true });
							vs_offset += 16;
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
							log_warn("Unknown shader stage %c", name.At(index));
						}
						index++;
					}
				}
#endif
			}
		};

		core::Array<sg_bindings> g_binds_array;
		core::Map<uint32_t, ShaderDescShaderHandle> MaterialShaders;

		static sx_mat4 g_mView, g_mProj, g_mViewProj, g_mWorld, g_mWorldViewProj;
		static sx_vec3 g_vLightDir, g_vLightPos, g_vCamPos;
		static sx_vec4 g_cLightColor;
		static bool g_bHasDirLight = false, g_bHasOmniLight = false;
		static core::RectI g_viewportSize;
		GfxSetup g_setup;

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

			sh.desc = ari_mesh_TNAD_shader_desc();
			sh.Setup("mesh_TNAD");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_TNAP_shader_desc();
			sh.Setup("mesh_TNAP");
			MaterialShaders.Add(sh.hash, sh);

			sh.desc = ari_mesh_TNMD_shader_desc();
			sh.Setup("mesh_TNMD");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_TNMP_shader_desc();
			sh.Setup("mesh_TNMP");
			MaterialShaders.Add(sh.hash, sh);

			sh.desc = ari_mesh_VCND_shader_desc();
			sh.Setup("mesh_VCND");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_VCNP_shader_desc();
			sh.Setup("mesh_VCNP");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_ND_shader_desc();
			sh.Setup("mesh_ND");
			MaterialShaders.Add(sh.hash, sh);
			sh.desc = ari_mesh_NP_shader_desc();
			sh.Setup("mesh_NP");
			MaterialShaders.Add(sh.hash, sh);
		}

		void SetMaterialShader(Material& material)
		{
			static core::StringBuilder str;
			str.Set("mesh_");
			if (material.HasTexcoord)
				str.Append("T"); // Add texture
			if (material.HasVertexColor)
				str.Append("VC"); // Add vertex color
			if (material.HasNormal && (g_bHasDirLight || g_bHasOmniLight))
				str.Append("N"); // Add Normal
			if (material.HasTexcoord && material.HasAlphaMap)
				str.Append("A");
			if (material.HasTexcoord && material.HasShadowAoSpecularMap)
				str.Append("M"); // Add Shadow, AO and specular map
			if (material.HasNormal && g_bHasDirLight)
				str.Append("D"); // Add directional light
			if (material.HasNormal && g_bHasOmniLight)
				str.Append("P"); // Add Point light

			uint32_t hash = sx_hash_xxh32(str.AsCStr(), str.Length(), 0);

			if (MaterialShaders.Contains(hash))
			{
				ShaderDescShaderHandle& mat = MaterialShaders[hash];
				if (!mat.shader.IsValid())
					mat.shader = CreateShader(mat.desc);
				if (material.shader.Handle == mat.shader.Handle)
					return;
				material.shader = mat.shader;
				material.FS_UniformSize = mat.FS_UniformSize;
				material.VS_UniformSize = mat.VS_UniformSize;
				material.Uniforms = &mat.Uniforms;
				material.Fs_UniformData.Reserve(material.FS_UniformSize - material.Fs_UniformData.Size());
				material.Fs_UniformData.FillDumyData();
				material.Vs_UniformData.Reserve(material.VS_UniformSize - material.Vs_UniformData.Size());
				material.Vs_UniformData.FillDumyData();
			}
			else
			{
				log_error("Shader for material %s not found.", str.AsCStr());
			}
		}

		//------------------------------------------------------------------------------
		ShaderHandle GetShader(ShaderType shader)
		{
			return g_shaders[int(shader)];
		}
		
		sx_vec4 g_clear_color = sx_vec4f(.5f, .5f, .5f, 1.0);
		void SetClearColor(const sx_vec4& _color)
		{
			g_clear_color = _color;
		}

		//------------------------------------------------------------------------------
		void SetViewportSize(const core::RectI& _rect)
		{
			g_viewportSize = _rect;
			sg_apply_viewport(_rect.x, _rect.y, _rect.width, _rect.height, false);
		}

		//------------------------------------------------------------------------------
		core::RectI GetViewportSize()
		{
			return g_viewportSize;
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
#ifdef ARI_SERVER
			uint32_t i = 0;
			return { core::HandleManager<ShaderHandle>::GetNewHandle(i), 0 };
#else
			const sg_shader shader = sg_make_shader(desc);
			return { core::HandleManager<ShaderHandle>::CreateHandleByIndex(shader.id), shader.id };
#endif
		}

		//------------------------------------------------------------------------------
		void DestroyShader(ShaderHandle& shader)
		{
			sg_destroy_shader({ shader.Index });
			core::HandleManager<ShaderHandle>::RemoveHandle(shader.Handle);
			shader.Handle = shader.Index = core::aInvalidHandle;
		}

		//------------------------------------------------------------------------------
		PipelineHandle CreatePipelineInternal(const PipelineSetup& setup)
		{
			sg_pipeline_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_pipeline_desc), 0);
			desc.shader.id = setup.shader.Index;
			for (int i = 0; i < ARI_MAX_SHADERSTAGE_BUFFERS; ++i)
			{
				desc.layout.buffers[i].step_func = sg_vertex_step(setup.layout.buffers[i].step);
				desc.layout.buffers[i].step_rate = setup.layout.buffers[i].stepRate;
				desc.layout.buffers[i].stride = setup.layout.buffers[i].stride;
			}
			for (int i = 0; i < ARI_MAX_VERTEX_ATTRIBUTES; ++i)
			{
				desc.layout.attrs[i].format = sg_vertex_format(setup.layout.attrs[i].format);
				desc.layout.attrs[i].buffer_index = setup.layout.attrs[i].bufferIndex;
				desc.layout.attrs[i].offset = setup.layout.attrs[i].offset;
			}
			desc.index_type = sg_index_type(setup.index_type);
			desc.rasterizer.cull_mode = SG_CULLMODE_BACK;
			desc.rasterizer.face_winding = SG_FACEWINDING_CCW;
			desc.depth_stencil.depth_write_enabled = true;
			desc.depth_stencil.depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL;

			// set blending
			desc.blend.enabled = setup.blend.enabled;
			desc.blend.src_factor_rgb = sg_blend_factor(setup.blend.src_factor_rgb);
			desc.blend.dst_factor_rgb = sg_blend_factor(setup.blend.dst_factor_rgb);
			desc.blend.op_rgb = sg_blend_op(setup.blend.op_rgb);
			desc.blend.src_factor_alpha = sg_blend_factor(setup.blend.src_factor_alpha);
			desc.blend.dst_factor_alpha = sg_blend_factor(setup.blend.dst_factor_alpha);
			desc.blend.op_alpha = sg_blend_op(setup.blend.op_alpha);
			desc.blend.color_write_mask = setup.blend.color_write_mask;
			desc.blend.color_attachment_count = setup.blend.color_attachment_count;
			desc.blend.color_format = sg_pixel_format(setup.blend.color_format);
			desc.blend.depth_format = sg_pixel_format(setup.blend.depth_format);
			core::Memory::Copy(setup.blend.blend_color, desc.blend.blend_color, 4 * sizeof(float));

			const sg_pipeline pipeline = sg_make_pipeline(&desc);
			return { core::HandleManager<PipelineHandle>::CreateHandleByIndex(pipeline.id), pipeline.id };
		}

		//------------------------------------------------------------------------------
		core::Array<core::KeyValuePair<gfx::PipelineSetup, gfx::PipelineHandle>> g_aPipelines;

		//------------------------------------------------------------------------------
		PipelineHandle CreatePipeline(const PipelineSetup& setup)
		{
			for (auto& pair : g_aPipelines)
			{
				const auto& p = pair.Key();
				if (p == setup)
					return pair.Value();
			}
			const auto pipe = gfx::CreatePipelineInternal(setup);
			g_aPipelines.Add({ setup, pipe });
			return pipe;
		}

		//------------------------------------------------------------------------------
		void DestroyPipeline(PipelineHandle& pipeline)
		{
			sg_destroy_pipeline({ pipeline.Index });
			for (int i = 0; i < g_aPipelines.Size(); i++)
			{
				if (g_aPipelines[i].Value().Handle == pipeline.Handle)
				{
					g_aPipelines.Erase(i);
					break;
				}
			}
			core::HandleManager<PipelineHandle>::RemoveHandle(pipeline.Handle);
			pipeline.Handle = pipeline.Index = core::aInvalidHandle;
		}

		//------------------------------------------------------------------------------
		void SetPipelineBlendState(PipelineHandle& pipeline, const BlendState& blend)
		{
			for (int i = 0; i < g_aPipelines.Size(); i++)
			{
				if (g_aPipelines[i].Value().Handle == pipeline.Handle)
				{
					if (!(g_aPipelines[i].key.blend == blend))
					{
						// create a new pipeline
						PipelineSetup setup = g_aPipelines[i].Key();
						setup.blend = blend;
						pipeline = CreatePipeline(setup);
					}
					return;
				}
			}
		}

		//------------------------------------------------------------------------------
		void SetPipelineShader(PipelineHandle& pipeline, const ShaderHandle& shader)
		{
			for (int i = 0; i < g_aPipelines.Size(); i++)
			{
				if (g_aPipelines[i].Value().Handle == pipeline.Handle)
				{
					if (g_aPipelines[i].key.shader.Handle != shader.Handle)
					{
						// create a new pipeline
						PipelineSetup setup = g_aPipelines[i].Key();
						setup.shader = shader;
						pipeline = CreatePipeline(setup);
					}
					return;
				}
			}
		}


		//------------------------------------------------------------------------------
		void ApplyPipeline(const PipelineHandle& pipeline)
		{
			sg_apply_pipeline({ pipeline.Index });
		}

		void SetUniformData(const MaterialUniformInfo& ui, Material* material, float* data)
		{
			if (ui.Stage == ShaderStage::VertexShader)
			{
				core::Memory::Copy(data, &material->Vs_UniformData[ui.Offset], ui.Size * 4);
			}
			else
			{
				core::Memory::Copy(data, &material->Fs_UniformData[ui.Offset], ui.Size * 4);
			}
		}

		//------------------------------------------------------------------------------
		void SetMaterialUniforms(Material* material)
		{
			for(int i = 0; i < material->Uniforms->Size(); i++)
			{
				const auto& ui = material->Uniforms->operator[](i);
				if (ui.Name == str_uni_mvp)
				{
					SetUniformData(ui, material, g_mWorldViewProj.f);
				}
				else if (ui.Name == str_uni_matWorld)
				{
					SetUniformData(ui, material, g_mWorld.f);
				}
				else if (ui.Name == str_uni_baseColor)
				{
					SetUniformData(ui, material, material->BaseColor.f);
				}
				else if (ui.Name == str_uni_camPos)
				{
					SetUniformData(ui, material, g_vCamPos.f);
				}
				else if (ui.Name == str_uni_lightDir)
				{
					SetUniformData(ui, material, g_vLightDir.f);
				}
				else if (ui.Name == str_uni_lightPos)
				{
					SetUniformData(ui, material, g_vLightPos.f);
				}
				else if (ui.Name == str_uni_lightColor)
				{
					SetUniformData(ui, material, g_cLightColor.f);
				}
				else if (ui.Name == str_uni_specularStrength)
				{
					SetUniformData(ui, material, &material->SpecularStrength);
				}
			}

		}

		//------------------------------------------------------------------------------
		void ApplyPipelineAndMaterial(PipelineHandle& pipeline, Material* material)
		{
			// update engine uniforms data
			SetMaterialUniforms(material);

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
			pass.colors->action = SG_ACTION_CLEAR;
			core::Memory::Copy(g_clear_color.f, pass.colors->val, sizeof(g_clear_color.f));

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
		TextureHandle LoadTexture(core::String _path)
		{
			sg_image img = sg_alloc_image();

			core::StringBuilder str = _path;
			bool isKtx = true;
			if (!str.Contains(".ktx"))
			{
				str.Append(".basis");
				isKtx = false;
			}
			io::LoadFile(str.GetString(), [img, isKtx](core::Buffer* buffer)
			{
				if (isKtx)
				{
					LoadKtxTexture(img, buffer);
				}
				else
				{
					LoadBasisTexture(img, buffer);
				}
			});

			return { core::HandleManager<TextureHandle>::CreateHandleByIndex(img.id), img.id };
		}

		uint32_t GetSurfacePitch(PixelFormat fmt, uint32_t width, uint32_t height, uint32_t row_align)
		{
			return _sg_surface_pitch((sg_pixel_format)fmt, width, height, row_align);
		}

		void SetDirLight(const sx_vec3& dir, const sx_vec4& color)
		{
			g_vLightDir = sx_vec3_norm(dir * -1);
			g_cLightColor = color;
			g_bHasDirLight = true;
			g_bHasOmniLight = false;
		}

		void SetOmniLight(const sx_vec3& pos, const sx_vec4& color)
		{
			g_vLightPos = pos;
			g_cLightColor = color;
			g_bHasDirLight = false;
			g_bHasOmniLight = true;
		}

		void SetCameraPosition(const sx_vec3& pos)
		{
			g_vCamPos = pos;
		}

		Mesh* GetMesh(MeshHandle& mesh_handle)
		{
			if (mesh_handle.IsValid())
				return core::ObjectPool<gfx::Mesh>::GetByIndex(mesh_handle.Handle);
			return nullptr;
		}

		SubMesh* GetSubMesh(SubMeshHandle& sub_mesh_handle)
		{
			if (sub_mesh_handle.IsValid())
				return core::ObjectPool<gfx::SubMesh>::GetByIndex(sub_mesh_handle.Handle);
			return nullptr;
		}
    } // namespace gfx
    
} // namespace ari
