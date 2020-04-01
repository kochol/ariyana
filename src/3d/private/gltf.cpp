#include "gltf.hpp"
#include "io/FileSystem.hpp"
#include "cgltf/cgltf.h"
#include "core/containers/Array.hpp"
#include "gfx/gfx.hpp"

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
												NumMeshes;
		core::Buffer*							Buffers;
		core::Array<gfx::BufferHandle>			GfxBuffers;
		core::Array<Accessor>					Accessors;
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
			accessor.Count = gltf_accessor->count;
			accessor.DataType = ComponentDataType(int(gltf_accessor->type));
			accessor.GfxBuffer = p_scene_data->GfxBuffers[index];
			accessor.HasMax = gltf_accessor->has_max;
			core::Memory::Copy(gltf_accessor->max, accessor.Max, sizeof(float) * 16);
			accessor.HasMin = gltf_accessor->has_min;
			core::Memory::Copy(gltf_accessor->min, accessor.Min, sizeof(float) * 16);
			accessor.Normalized = gltf_accessor->normalized;
			accessor.Offset = gltf_accessor->offset;
			p_scene_data->Accessors.Add(accessor);
		}

		// free the gltf pointer
		cgltf_free(gltf);
	}

	void LoadGltfScene(const core::String& _path, std::function<void(core::Array<ComponentHandle<Node3D>>)> OnModel)
	{
		// Load gltf file
		io::LoadFile(_path, [_path](core::Buffer* buffer)
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
						io::LoadFile(gltf_buf->uri, [i, gltf, p_scene_data](core::Buffer* buffer)
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
