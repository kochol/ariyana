#include "gltf.hpp"
#include "io/FileSystem.hpp"
#include "cgltf/cgltf.h"
#include "core/containers/Array.hpp"
#include "gfx/gfx.hpp"
#include "gfx/Mesh.hpp"
#include "core/string/StringBuilder.hpp"
#include "3d/MeshNode.hpp"
#include "3d/Camera.hpp"
#include "core/log.h"

namespace ari::en
{
	enum class ComponentTypeEnum
	{
		invalid,
		s8, /* BYTE */
		u8, /* UNSIGNED_BYTE */
		s16, /* SHORT */
		u16, /* UNSIGNED_SHORT */
		u32, /* UNSIGNED_INT */
		f32, /* FLOAT */
	};

	enum class ComponentDataType
	{
		invalid,
		scalar,
		vec2,
		vec3,
		vec4,
		mat2,
		mat3,
		mat4,
	};

	struct Accessor
	{
		gfx::BufferHandle	GfxBuffer;
		int					Offset = 0;
		ComponentTypeEnum	ComponentType;
		bool				Normalized = false;
		int					Count;
		ComponentDataType	DataType;
		bool				HasMin;
		float				Min[16];
		bool				HasMax;
		float				Max[16];
	};

	struct SceneData
	{
		int										NumBuffers,
												NumLoadedBuffers = 0,
												NumBufferViews,
												NumImages,
												NumMaterials,
												NumMeshes,
												NumNodes;
		core::Buffer*							Buffers;
		core::Array<gfx::BufferHandle>			GfxBuffers;
		core::Array<Accessor>					Accessors;
		core::Array<gfx::MeshHandle>			Meshes;
		core::Array<gfx::TextureHandle>			Textures;
		core::String							BasePath;
	};

	void SetPipelineAttribute(gfx::VertexAttrSetup& attr, cgltf_attribute* gltf_attr)
	{
		switch (gltf_attr->data->component_type)
		{
		case cgltf_component_type_invalid: break;

		case cgltf_component_type_r_8:
			if (gltf_attr->data->normalized)
				attr.format = gfx::VertexFormat::Byte4N;
			else
				attr.format = gfx::VertexFormat::Byte4;
			break;

		case cgltf_component_type_r_8u:
			if (gltf_attr->data->normalized)
				attr.format = gfx::VertexFormat::UByte4N;
			else
				attr.format = gfx::VertexFormat::UByte4;
			break;

		case cgltf_component_type_r_16:
			if (gltf_attr->data->type == cgltf_type_vec2)
			{
				if (gltf_attr->data->normalized)
					attr.format = gfx::VertexFormat::Short2;
				else
					attr.format = gfx::VertexFormat::Short2N;
			}
			else //if (gltf_attr->data->type == cgltf_type_vec4)
			{
				if (gltf_attr->data->normalized)
					attr.format = gfx::VertexFormat::Short4;
				else
					attr.format = gfx::VertexFormat::Short4N;
			}
			break;
			
		case cgltf_component_type_r_16u: 
			if (gltf_attr->data->type == cgltf_type_vec2)
			{
				if (gltf_attr->data->normalized)
					attr.format = gfx::VertexFormat::Short2;
				else
					attr.format = gfx::VertexFormat::Short2N;
			}
			else //if (gltf_attr->data->type == cgltf_type_vec4)
			{
				if (gltf_attr->data->normalized)
					attr.format = gfx::VertexFormat::Short4;
				else
					attr.format = gfx::VertexFormat::Short4N;
			}
			break;

		case cgltf_component_type_r_32u: 
		case cgltf_component_type_r_32f:
			{
				switch (gltf_attr->data->type) 
				{ 
				case cgltf_type_scalar: attr.format = gfx::VertexFormat::Float; break;
				case cgltf_type_vec2: attr.format = gfx::VertexFormat::Float2; break;
				case cgltf_type_vec3: attr.format = gfx::VertexFormat::Float3; break;
				case cgltf_type_vec4: attr.format = gfx::VertexFormat::Float4; break;
				}
			}
			break;
		}
	}

	void SetNodeTransform(cgltf_node* node, Node3D* n)
	{
		if (node->has_matrix)
		{
			core::Memory::Copy(node->matrix, n->Transform.f, 64);
			n->has_mat = true;
		}
		else
		{
			n->Position.f[0] = node->translation[0];
			n->Position.f[1] = node->translation[1];
			n->Position.f[2] = node->translation[2];
			n->Rotation.f[0] = node->rotation[0];
			n->Rotation.f[1] = node->rotation[1];
			n->Rotation.f[2] = node->rotation[2];
			n->Rotation.f[3] = node->rotation[3];
			n->Scale.f[0] = node->scale[0];
			n->Scale.f[1] = node->scale[1];
			n->Scale.f[2] = node->scale[2];
		}
	}

	ComponentHandle<Node3D> CreateNode(cgltf_data* gltf, SceneData* p_scene_data, cgltf_node* node)
	{
		ComponentHandle<Node3D> n;
		n.Component = nullptr;
		if (node->mesh)
		{
			// create a mesh node
			const int mesh_index = int(node->mesh - gltf->meshes);
			auto m = World::CreateComponent<MeshNode, Node3D>();
			m.Component->Mesh = p_scene_data->Meshes[mesh_index];
			SetNodeTransform(node, m.Component);
			n = CastComponentHandle<MeshNode, Node3D>(m);
		}
		else if (node->camera)
		{
			// create a camera node
			auto c = World::CreateComponent<Camera, Node3D>();
			SetNodeTransform(node, c.Component);
			if (node->camera->type == cgltf_camera_type_perspective)
			{
				c.Component->Type = CameraType::Perspective;
				c.Component->AspectRatio = node->camera->perspective.aspect_ratio;
				c.Component->Fov = node->camera->perspective.yfov;
				c.Component->zNear = node->camera->perspective.znear;
				c.Component->zFar = node->camera->perspective.zfar;
			}
			else if (node->camera->type == cgltf_camera_type_orthographic)
			{
				c.Component->Type = CameraType::Orthographic;
				c.Component->xMag = node->camera->orthographic.xmag;
				c.Component->yMag = node->camera->orthographic.ymag;
				c.Component->zNear = node->camera->orthographic.znear;
				c.Component->zFar = node->camera->orthographic.zfar;
			}
			else
			{
				log_error("Invalid camera node in gltf file");
			}
			n = CastComponentHandle<Camera, Node3D>(c);
		}
		else if (node->children_count > 0)
		{
			// create an empty node
			n = World::CreateComponent<Node3D, Node3D>();
			SetNodeTransform(node, n.Component);
		}

		// Add children
		if (n.Component)
		{
			for (cgltf_size i = 0; i < node->children_count; ++i)
			{
				const auto c = CreateNode(gltf, p_scene_data, node->children[i]);
				if (c.Component)
					n.Component->AddChild(c.Component);
			}
		}

		return n;
	}

	bool gltf_parse(cgltf_data* gltf, SceneData* p_scene_data, 
		const std::function<void(core::Array<ComponentHandle<Node3D>>)>& OnModel)
	{
		if (p_scene_data->NumLoadedBuffers < p_scene_data->NumBuffers)
			return false;

		// parse the buffer views
		p_scene_data->NumBufferViews = int(gltf->buffer_views_count);
		for (int i = 0; i < p_scene_data->NumBufferViews; i++)
		{
			const cgltf_buffer_view* gltf_buf_view = &gltf->buffer_views[i];
			const int buffer_index = int(gltf_buf_view->buffer - gltf->buffers);
			core::Buffer* buffer = &p_scene_data->Buffers[buffer_index];
			const int offset = int(gltf_buf_view->offset);
			const int size = int(gltf_buf_view->size);
			a_assert((offset + size) <= buffer->Size());

			if (gltf_buf_view->type == cgltf_buffer_view_type_indices)
			{
				p_scene_data->GfxBuffers.Add(gfx::CreateIndexBuffer(size, buffer->Data() + offset));
			}
			else if (gltf_buf_view->type == cgltf_buffer_view_type_vertices)
			{
				p_scene_data->GfxBuffers.Add(gfx::CreateVertexBuffer(size, buffer->Data() + offset));
			}
			else
			{
				p_scene_data->GfxBuffers.Add(gfx::BufferHandle());
			}
		}

		// parse the Accessors
		for (int i = 0; i < int(gltf->accessors_count); i++)
		{
			const cgltf_accessor* gltf_accessor = &gltf->accessors[i];
			const int index = int(gltf_accessor->buffer_view - gltf->buffer_views);
			Accessor accessor;
			accessor.ComponentType = ComponentTypeEnum(int(gltf_accessor->component_type));
			accessor.Count = int(gltf_accessor->count);
			accessor.DataType = ComponentDataType(int(gltf_accessor->type));
			// Check for sparse buffers
			if (gltf_accessor->is_sparse)
			{
				// create new buffer				
				// get indices buffer
				const cgltf_buffer_view* gltf_buf_view_indices = gltf_accessor->sparse.indices_buffer_view;
				const int buffer_index_indices = int(gltf_buf_view_indices->buffer - gltf->buffers);
				core::Buffer* buffer_indices = &p_scene_data->Buffers[buffer_index_indices];

				// get vertices buffer
				const cgltf_buffer_view* gltf_buf_view_values = gltf_accessor->sparse.values_buffer_view;
				const int buffer_index_values = int(gltf_buf_view_values->buffer - gltf->buffers);
				core::Buffer* buffer_values = &p_scene_data->Buffers[buffer_index_values];

				// copy original buffer
				const cgltf_buffer_view* gltf_buf_view_orig = gltf_accessor->buffer_view;
				const int buffer_index_orig = int(gltf_buf_view_orig->buffer - gltf->buffers);
				core::Buffer* buffer_orig = &p_scene_data->Buffers[buffer_index_orig];
				core::Buffer new_buffer;
				new_buffer.Add(buffer_orig->Data() + gltf_buf_view_orig->offset, int(gltf_buf_view_orig->size));

				// change the requested data
				int values_size = int(gltf_buf_view_values->size / gltf_accessor->sparse.count);
				for (int i = 0; i < int(gltf_accessor->sparse.count); ++i)
				{
					uint16_t* idx = reinterpret_cast<uint16_t*>(buffer_indices->Data() 
						+ gltf_accessor->sparse.indices_byte_offset 
						+ gltf_buf_view_indices->offset 
						+ i * 2);
					uint8_t* data = buffer_values->Data()
						+ gltf_accessor->sparse.values_byte_offset
						+ gltf_buf_view_values->offset
						+ i * values_size;
					core::Memory::Copy(data, new_buffer.Data() + (*idx * values_size), values_size);
				}
				// create new gfx buffer
				auto gfx_buffer = gfx::CreateVertexBuffer(new_buffer.Size(), new_buffer.Data());
				p_scene_data->GfxBuffers.Add(gfx_buffer);
				accessor.GfxBuffer = gfx_buffer;
			}
			else
			{
				accessor.GfxBuffer = p_scene_data->GfxBuffers[index];
			}
			accessor.HasMax = gltf_accessor->has_max;
			core::Memory::Copy(gltf_accessor->max, accessor.Max, sizeof(float) * 16);
			accessor.HasMin = gltf_accessor->has_min;
			core::Memory::Copy(gltf_accessor->min, accessor.Min, sizeof(float) * 16);
			accessor.Normalized = gltf_accessor->normalized;
			accessor.Offset = int(gltf_accessor->offset);
			p_scene_data->Accessors.Add(accessor);
		}

		// parse the textures
		for (size_t i = 0; i < gltf->textures_count; i++)
		{
			auto tex = &gltf->textures[i];
			core::StringBuilder string_builder(p_scene_data->BasePath);
			string_builder.Append(tex->image->uri);
			p_scene_data->Textures.Add(gfx::LoadTexture(string_builder.AsCStr()));
			
		}
		

		// parse the materials
		/*for (size_t i = 0; i < gltf->materials_count; i++)
		{
			auto mat = &gltf->materials[i];
			mat->
		}*/
		

		// parse the meshes
		core::ObjectPool<gfx::Mesh>::Setup(64);
		core::ObjectPool<gfx::SubMesh>::Setup(128);
		p_scene_data->NumMeshes = int(gltf->meshes_count);
		p_scene_data->Meshes.Reserve(p_scene_data->NumMeshes);
		for (int i = 0; i < p_scene_data->NumMeshes; i++)
		{
			const cgltf_mesh* gltf_mesh = &gltf->meshes[i];

			gfx::MeshHandle mesh_handle;
			mesh_handle.Handle = core::HandleManager<gfx::MeshHandle>::GetNewHandle(mesh_handle.Index);
			auto mesh = core::ObjectPool<gfx::Mesh>::New(mesh_handle.Index);
			mesh->SubMeshes.Reserve(int(gltf_mesh->primitives_count));
			p_scene_data->Meshes.Add(mesh_handle);

			for (cgltf_size prim_index = 0; prim_index < gltf_mesh->primitives_count; prim_index++)
			{
				const cgltf_primitive* gltf_prim = &gltf_mesh->primitives[prim_index];
				gfx::SubMeshHandle sub_mesh_handle;
				sub_mesh_handle.Handle = core::HandleManager<gfx::SubMeshHandle>::GetNewHandle(sub_mesh_handle.Index);
				auto sub_mesh = core::ObjectPool<gfx::SubMesh>::New(sub_mesh_handle.Index);
				mesh->SubMeshes.Add(sub_mesh_handle);
				gfx::PipelineSetup pipeline_setup;
				gfx::Bindings bindings;

				// Set the textures
				if (gltf_prim->material 
					&& gltf_prim->material->has_pbr_metallic_roughness
					&& gltf_prim->material->pbr_metallic_roughness.base_color_texture.texture)
				{
					int tex_index = gltf_prim->material->pbr_metallic_roughness.base_color_texture.texture - gltf->textures;
					bindings.fsTextures[0] = p_scene_data->Textures[tex_index];						
					pipeline_setup.shader = gfx::GetShader(gfx::ShaderType::BasicTexture);
				}
				else
				{
					pipeline_setup.shader = gfx::GetShader(gfx::ShaderType::Basic);
				}
				
				sub_mesh->Type = gfx::PrimitiveType(int(gltf_prim->type));
				if (gltf_prim->indices)
				{
					// Add indices
					const int accessor_index = int(gltf_prim->indices - gltf->accessors);
					sub_mesh->IndexBuffer = p_scene_data->Accessors[accessor_index].GfxBuffer;
					sub_mesh->ElementsCount = int(gltf_prim->indices->count);
					pipeline_setup.index_type = gfx::IndexType::Uint16;
					bindings.indexBuffer = sub_mesh->IndexBuffer;
				}
				else
					sub_mesh->ElementsCount = int(gltf_prim->attributes[0].data->count);

				// Set the buffers
				for (int a_i = 0; a_i < int(gltf_prim->attributes_count); ++a_i)
				{
					const int accessor_index = int(gltf_prim->attributes[a_i].data - gltf->accessors);

					int buffer_index = -1;
					Accessor* accessor = &p_scene_data->Accessors[accessor_index];

					switch (gltf_prim->attributes[a_i].type)
					{
					case cgltf_attribute_type_position:
						sub_mesh->Position = accessor->GfxBuffer;
						buffer_index = 0;
						if (accessor->HasMax)
						{
							sub_mesh->AABB.xmax = accessor->Max[0];
							sub_mesh->AABB.ymax = accessor->Max[1];
							sub_mesh->AABB.zmax = accessor->Max[2];
							sub_mesh->AABB.xmin = accessor->Min[0];
							sub_mesh->AABB.ymin = accessor->Min[1];
							sub_mesh->AABB.zmin = accessor->Min[2];
						}
						break;
					case cgltf_attribute_type_texcoord:
						sub_mesh->Texcoord = accessor->GfxBuffer;
						if (sub_mesh->Material.HasVertexColor)
							continue;
						buffer_index = 1;
						sub_mesh->Material.HasTexcoord = true;
						break;
					case cgltf_attribute_type_normal:
						continue;
						sub_mesh->Normal = accessor->GfxBuffer;
						buffer_index = 2;
						break;
					case cgltf_attribute_type_tangent:
						sub_mesh->Tangent = accessor->GfxBuffer;
						buffer_index = 3;
						break;
					case cgltf_attribute_type_color:
						sub_mesh->Color = p_scene_data->Accessors[accessor_index].GfxBuffer;
						buffer_index = 1;
						pipeline_setup.shader = gfx::GetShader(gfx::ShaderType::BasicVertexColor);
						sub_mesh->Material.HasVertexColor = true;
						sub_mesh->Material.HasTexcoord = false;
						break;
					case cgltf_attribute_type_joints:
						sub_mesh->Joints = p_scene_data->Accessors[accessor_index].GfxBuffer;
						break;
					case cgltf_attribute_type_weights:
						sub_mesh->Weights = p_scene_data->Accessors[accessor_index].GfxBuffer;
						break;
					case cgltf_attribute_type_invalid:
						break;
					}

					if (buffer_index >= 0)
					{
						// setup pipeline and bindings
						SetPipelineAttribute(pipeline_setup.layout.attrs[buffer_index], &gltf_prim->attributes[a_i]);
						pipeline_setup.layout.attrs[buffer_index].bufferIndex = buffer_index;
						bindings.vertexBuffers[buffer_index] = accessor->GfxBuffer;
						bindings.vertexBufferOffsets[buffer_index] = accessor->Offset;
					}
				}

				sub_mesh->Pipeline = gfx::CreatePipeline(pipeline_setup);
				sub_mesh->Binding = gfx::CreateBinding(bindings);
			}
		}

		core::Array<ComponentHandle<Node3D>> result;

		// parse nodes & scenes
		p_scene_data->NumNodes = int(gltf->scenes_count);
		for (int i = 0; i < p_scene_data->NumNodes; ++i)
		{
			auto scene = &gltf->scenes[i];

			for (int n = 0; n < int(scene->nodes_count); ++n)
			{
				auto node = scene->nodes[n];
				auto n3d = CreateNode(gltf, p_scene_data, node);
				if (n3d.Component)
					result.Add(n3d);
			}			 
		}

		// free the gltf pointer
		cgltf_free(gltf);

		// callback the user
		OnModel(result);

		return true;
	}

	void LoadGltfScene(const core::String& _path, std::function<void(core::Array<ComponentHandle<Node3D>>)> OnModel)
	{
		// Load gltf file
		io::LoadFile(_path, [_path, OnModel](core::Buffer* buffer)
			{
				cgltf_options options;
				core::Memory::Fill(&options, sizeof(cgltf_options), 0);
				cgltf_data* gltf = nullptr;
				const cgltf_result result = cgltf_parse(&options, buffer->Data(), buffer->Size(), &gltf);
				if (result == cgltf_result_success) 
				{
					SceneData* p_scene_data = core::Memory::New<SceneData>();

					// get the root path
					core::StringBuilder strPath(_path);
					int slash_index = strPath.FindLastOf(0, core::EndOfString, "/");
					if (slash_index < 0)
						slash_index = strPath.FindLastOf(0, core::EndOfString, ":");
					strPath.Set(strPath.GetSubString(0, slash_index + 1));
					p_scene_data->BasePath = strPath.AsCStr();

					p_scene_data->Buffers = core::Memory::NewClassArray<core::Buffer>(int(gltf->buffers_count));
					p_scene_data->NumBuffers = int(gltf->buffers_count);

					// start loading all buffers
					for (cgltf_size i = 0; i < gltf->buffers_count; i++)
					{
						const cgltf_buffer* gltf_buf = &gltf->buffers[i];
						if (strncmp(gltf_buf->uri, "data:", 5) == 0)
						{
							const char* comma = strchr(gltf_buf->uri, ',');

							if (comma && comma - gltf_buf->uri >= 7 && strncmp(comma - 7, ";base64", 7) == 0)
							{
								// the data is embedded in base64 encoding
								core::Buffer bufferData;
								bufferData.Reserve(int(gltf_buf->size));
								void* data;
								cgltf_result res = cgltf_load_buffer_base64(&options, gltf_buf->size, comma + 1, &data);

								if (res != cgltf_result_success)
								{
									// TODO: Log error
								}

								bufferData.Add(reinterpret_cast<uint8_t*>(data), int(gltf_buf->size));
								core::Memory::Free(data);
								p_scene_data->Buffers[i] = std::move(bufferData);
								p_scene_data->NumLoadedBuffers++;
								if (gltf_parse(gltf, p_scene_data, OnModel))
									return;
							}
							else
							{
								// return cgltf_result_unknown_format;
								// TODO: Log error
							}
						}
						else
						{
							core::StringBuilder string_builder(strPath.AsCStr());
							string_builder.Append(gltf_buf->uri);
							io::LoadFile(string_builder.AsCStr(), [i, gltf, p_scene_data, OnModel](core::Buffer* buffer)
								{
									p_scene_data->Buffers[i] = std::move(*buffer);
									p_scene_data->NumLoadedBuffers++;
									gltf_parse(gltf, p_scene_data, OnModel);
								});
						}
					}
				}

			});
	}
} // namespace ari::en
