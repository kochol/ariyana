#include "3dModel.hpp"
#include "io/FileSystem.hpp"
#include "private/cgltf/cgltf.h"
#include <sokol_gfx.h>
#include "gfx/gfx.hpp"

namespace ari::en
{
	// resource creation helper params, these are stored until the
	// async-loaded resources (buffers and images) have been loaded
	struct buffer_creation_params_t {
		bool is_vertex_buffer;
		int offset;
		int size;
		int gltf_buffer_index;
	};

	// Store the needed data for creating the mesh
	struct MeshData
	{
		int NumBuffers;
		core::Array<buffer_creation_params_t> BufferParams;
		core::Array<gfx::BufferHandle> VertexBuffers;
		core::Array<gfx::BufferHandle> IndexBuffers;
	};

	//------------------------------------------------------------------------------
	// compute indices from cgltf element pointers
	//------------------------------------------------------------------------------
	static int gltf_buffer_index(const cgltf_data* gltf, const cgltf_buffer* buf) {
		assert(buf);
		return int(buf - gltf->buffers);
	}

	//------------------------------------------------------------------------------
	// parse the GLTF buffer definitions and start loading buffer blobs
	//------------------------------------------------------------------------------
	static void gltf_parse_buffers(const cgltf_data* gltf, MeshData* p_mesh_data)
	{
		// parse the buffer-view attributes
		p_mesh_data->NumBuffers = (int)gltf->buffer_views_count;
		for (int i = 0; i < p_mesh_data->NumBuffers; i++) 
		{
			const cgltf_buffer_view* gltf_buf_view = &gltf->buffer_views[i];
			buffer_creation_params_t p;
			p.gltf_buffer_index = gltf_buffer_index(gltf, gltf_buf_view->buffer);
			p.offset = int(gltf_buf_view->offset);
			p.size = int(gltf_buf_view->size);
			if (gltf_buf_view->type == cgltf_buffer_view_type_indices) {
				p.is_vertex_buffer = false;
			}
			else {
				p.is_vertex_buffer = true;
			}
			p_mesh_data->BufferParams.Add(p);
		}

		// start loading all buffers
		for (cgltf_size i = 0; i < gltf->buffers_count; i++) 
		{
			const cgltf_buffer* gltf_buf = &gltf->buffers[i];
			io::LoadFile(gltf_buf->uri, [i, p_mesh_data](core::Buffer* buffer)
				{
					for (int c = 0; c < p_mesh_data->NumBuffers; c++) 
					{
						const buffer_creation_params_t* p = &p_mesh_data->BufferParams[c];
						if (p->gltf_buffer_index == int(i)) 
						{
							a_assert((p->offset + p->size) <= buffer->Size());
							if (p->is_vertex_buffer)
								p_mesh_data->VertexBuffers.Add(gfx::CreateVertexBuffer(p->size, buffer->Data() + p->offset));
							else
								p_mesh_data->IndexBuffers.Add(gfx::CreateIndexBuffer(p->size, buffer->Data() + p->offset));
						}
					}
				});
		}
	}

	//------------------------------------------------------------------------------
	// load GLTF data from memory, build scene and issue resource fetch requests
	//------------------------------------------------------------------------------
	static void gltf_parse(const void* ptr, uint64_t num_bytes, MeshData* p_mesh_data)
	{
		cgltf_options options;
		core::Memory::Fill(&options, sizeof(cgltf_options), 0);
		cgltf_data* data = nullptr;
		const cgltf_result result = cgltf_parse(&options, ptr, num_bytes, &data);
		if (result == cgltf_result_success) {
			gltf_parse_buffers(data, p_mesh_data);
			gltf_parse_images(data);
			gltf_parse_materials(data);
			gltf_parse_meshes(data);
			gltf_parse_nodes(data);
			cgltf_free(data);
		}
	}

	//------------------------------------------------------------------------------
	void Load3dModel(core::String _path, std::function<void(ComponentHandle<Node3D>)> OnModel)
	{
		io::LoadFile(_path, [](core::Buffer* buffer)
			{
				MeshData mesh_data;
				gltf_parse((void*)buffer->Data(), buffer->Size(), &mesh_data);
			});
	} // Load3dModel

} // namespace ari::en
