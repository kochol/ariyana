#include "3dModel.hpp"
#include "io/FileSystem.hpp"
#include "private/cgltf/cgltf.h"

namespace ari::en
{
	//------------------------------------------------------------------------------
	// parse the GLTF buffer definitions and start loading buffer blobs
	static void gltf_parse_buffers(const cgltf_data* gltf) {
		if (gltf->buffer_views_count > SCENE_MAX_BUFFERS) {
			state.failed = true;
			return;
		}

		// parse the buffer-view attributes
		state.scene.num_buffers = (int)gltf->buffer_views_count;
		for (int i = 0; i < state.scene.num_buffers; i++) {
			const cgltf_buffer_view* gltf_buf_view = &gltf->buffer_views[i];
			buffer_creation_params_t* p = &state.creation_params.buffers[i];
			p->gltf_buffer_index = gltf_buffer_index(gltf, gltf_buf_view->buffer);
			p->offset = (int)gltf_buf_view->offset;
			p->size = (int)gltf_buf_view->size;
			if (gltf_buf_view->type == cgltf_buffer_view_type_indices) {
				p->type = SG_BUFFERTYPE_INDEXBUFFER;
			}
			else {
				p->type = SG_BUFFERTYPE_VERTEXBUFFER;
			}
			// allocate a sokol-gfx buffer handle
			state.scene.buffers[i] = sg_alloc_buffer();
		}

		// start loading all buffers
		for (cgltf_size i = 0; i < gltf->buffers_count; i++) {
			const cgltf_buffer* gltf_buf = &gltf->buffers[i];
			gltf_buffer_fetch_userdata_t user_data = {
				.buffer_index = i
			};
			sfetch_send(&(sfetch_request_t) {
				.path = gltf_buf->uri,
					.callback = gltf_buffer_fetch_callback,
					.user_data_ptr = &user_data,
					.user_data_size = sizeof(user_data)
			});
		}
	}

	//------------------------------------------------------------------------------
	// load GLTF data from memory, build scene and issue resource fetch requests
	static void gltf_parse(const void* ptr, uint64_t num_bytes) {
		cgltf_options options;
		core::Memory::Fill(&options, sizeof(cgltf_options), 0);
		cgltf_data* data = nullptr;
		const cgltf_result result = cgltf_parse(&options, ptr, num_bytes, &data);
		if (result == cgltf_result_success) {
			gltf_parse_buffers(data);
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
				gltf_parse((void*)buffer->Data(), buffer->Size());
			});
	} // Load3dModel

} // namespace ari::en
