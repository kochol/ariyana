#include "gltf.hpp"
#include "io/FileSystem.hpp"
#include "cgltf/cgltf.h"
#include "core/containers/Array.hpp"
#include "gfx/gfx.hpp"
#include "gfx/Mesh.hpp"

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
	};

	void gltf_parse(cgltf_data* gltf, SceneData* p_scene_data)
	{
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
			else
			{
				p_scene_data->GfxBuffers.Add(gfx::CreateVertexBuffer(size, buffer->Data() + offset));
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
			accessor.GfxBuffer = p_scene_data->GfxBuffers[index];
			accessor.HasMax = gltf_accessor->has_max;
			core::Memory::Copy(gltf_accessor->max, accessor.Max, sizeof(float) * 16);
			accessor.HasMin = gltf_accessor->has_min;
			core::Memory::Copy(gltf_accessor->min, accessor.Min, sizeof(float) * 16);
			accessor.Normalized = gltf_accessor->normalized;
			accessor.Offset = int(gltf_accessor->offset);
			p_scene_data->Accessors.Add(accessor);
		}

		// parse the meshes
		core::ObjectPool<gfx::Mesh>::Setup(64);
		core::ObjectPool<gfx::SubMesh>::Setup(128);
		p_scene_data->NumMeshes = int(gltf->meshes_count);
		p_scene_data->Meshes.Reserve(p_scene_data->NumMeshes);
		for (int i = 0; i < p_scene_data->NumMeshes; i++)
		{
			const cgltf_mesh* gltf_mesh = &gltf->meshes[i];

			gfx::MeshHandle mesh_handle;
			mesh_handle.Handle = core::HandleManager<gfx::Mesh>::GetNewHandle(mesh_handle.Index);
			auto mesh = core::ObjectPool<gfx::Mesh>::New(mesh_handle.Index);
			mesh->SubMeshes.Reserve(int(gltf_mesh->primitives_count));
			p_scene_data->Meshes.Add(mesh_handle);

			for (cgltf_size prim_index = 0; prim_index < gltf_mesh->primitives_count; prim_index++)
			{
				const cgltf_primitive* gltf_prim = &gltf_mesh->primitives[prim_index];
				gfx::SubMeshHandle sub_mesh_handle;
				sub_mesh_handle.Handle = core::HandleManager<gfx::SubMesh>::GetNewHandle(sub_mesh_handle.Index);
				auto sub_mesh = core::ObjectPool<gfx::SubMesh>::New(sub_mesh_handle.Index);
				mesh->SubMeshes.Add(sub_mesh_handle);

				sub_mesh->Type = gfx::PrimitiveType(int(gltf_prim->type));
				if (gltf_prim->indices)
				{
					// Add indices
					const int accessor_index = int(gltf_prim->indices - gltf->accessors);
					sub_mesh->IndexBuffer = p_scene_data->Accessors[accessor_index].GfxBuffer;
					sub_mesh->ElementsCount = int(gltf_prim->indices->count);
				}
				else
					sub_mesh->ElementsCount = int(gltf_prim->attributes[0].data->count);

				// Set the buffers
				for (int a_i = 0; a_i < int(gltf_prim->attributes_count); ++a_i)
				{
					const int accessor_index = int(gltf_prim->attributes[a_i].data - gltf->accessors);
					switch (gltf_prim->attributes[a_i].type)
					{
					case cgltf_attribute_type_position:
						sub_mesh->Position = p_scene_data->Accessors[accessor_index].GfxBuffer;
						// TODO: add bounding box
						break;
					case cgltf_attribute_type_normal:
						sub_mesh->Normal = p_scene_data->Accessors[accessor_index].GfxBuffer;
						break;
					case cgltf_attribute_type_tangent:
						sub_mesh->Tangent = p_scene_data->Accessors[accessor_index].GfxBuffer;
						break;
					case cgltf_attribute_type_texcoord:
						sub_mesh->Texcoord = p_scene_data->Accessors[accessor_index].GfxBuffer;
						break;
					case cgltf_attribute_type_color:
						sub_mesh->Color = p_scene_data->Accessors[accessor_index].GfxBuffer;
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
				}
			}
		}

		// parse nodes
		p_scene_data->NumNodes = int(gltf->scene->nodes_count);
		for (int i = 0; i < p_scene_data->NumNodes; ++i)
		{
			
		}

		// free the gltf pointer
		cgltf_free(gltf);
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
					p_scene_data->Buffers = reinterpret_cast<core::Buffer*>(core::Memory::Alloc(int(sizeof(core::Buffer) * gltf->buffers_count)));
					p_scene_data->NumBuffers = int(gltf->buffers_count);

					// start loading all buffers
					for (cgltf_size i = 0; i < gltf->buffers_count; i++)
					{
						const cgltf_buffer* gltf_buf = &gltf->buffers[i];
						io::LoadFile(gltf_buf->uri, [i, gltf, p_scene_data, OnModel](core::Buffer* buffer)
							{
								p_scene_data->Buffers[i] = std::move(*buffer);
								p_scene_data->NumLoadedBuffers++;
								if (p_scene_data->NumLoadedBuffers == p_scene_data->NumBuffers)
								{
									// Buffer loading finished do the reset
									gltf_parse(gltf, p_scene_data);
								}
							});
					}
				}

			});
	}
} // namespace ari::en
