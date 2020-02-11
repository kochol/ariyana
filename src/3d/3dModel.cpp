#include "3dModel.hpp"
#include "io/FileSystem.hpp"
#include "private/cgltf/cgltf.h"
#include <sokol_gfx.h>
#include "gfx/gfx.hpp"

namespace ari::en
{
	// resource creation helper params, these are stored until the
	// async-loaded resources (buffers and images) have been loaded
	struct buffer_creation_params_t 
	{
		bool is_vertex_buffer;
		int offset;
		int size;
		int gltf_buffer_index;
	};

	struct image_creation_params_t 
	{
		gfx::TextureFilter min_filter;
		gfx::TextureFilter mag_filter;
		gfx::TextureWrap wrap_s;
		gfx::TextureWrap wrap_t;
		int gltf_image_index;
	};

	// Store the needed data for creating the mesh
	struct MeshData
	{
		int										NumBuffers, 
												NumImages;
		core::Array<buffer_creation_params_t>	BufferParams;
		core::Array<gfx::BufferHandle>			VertexBuffers,
												IndexBuffers;
		core::Array<image_creation_params_t>	ImageParams;
		core::Array<gfx::TextureHandle>			Textures;
		bool									BufferLoaded = false;
	};

	//------------------------------------------------------------------------------
	// compute indices from cgltf element pointers
	//------------------------------------------------------------------------------
	static int gltf_buffer_index(const cgltf_data* gltf, const cgltf_buffer* buf) {
		a_assert(buf);
		return int(buf - gltf->buffers);
	}

	//------------------------------------------------------------------------------
	static int gltf_image_index(const cgltf_data* gltf, const cgltf_image* img) {
		a_assert(img);
		return (int)(img - gltf->images);
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
					p_mesh_data->BufferLoaded = true;
				});
		}
	}

	// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#samplerminfilter
	static gfx::TextureFilter gltf_to_sg_filter(int gltf_filter) 
	{
		switch (gltf_filter)
		{
			case 9728: return gfx::TextureFilter::Nearest;
			case 9729: return gfx::TextureFilter::Linear;
			case 9984: return gfx::TextureFilter::NearestMipmapNearest;
			case 9985: return gfx::TextureFilter::LinearMipmapNearest;
			case 9986: return gfx::TextureFilter::NearestMipmapLinear;
			case 9987: return gfx::TextureFilter::LinearMipmapLinear;
			default: return gfx::TextureFilter::Linear;
		}
	}

	// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#samplerwraps
	static gfx::TextureWrap gltf_to_sg_wrap(int gltf_wrap) 
	{
		switch (gltf_wrap) 
		{
			case 33071: return gfx::TextureWrap::ClampToEdge;
			case 33648: return gfx::TextureWrap::MirroredRepeat;
			case 10497: return gfx::TextureWrap::Repeat;
			default: return gfx::TextureWrap::Repeat;
		}
	}

	//------------------------------------------------------------------------------
	static void gltf_parse_images(const cgltf_data* gltf, MeshData* p_mesh_data)
	{
		// parse the texture and sampler attributes
		p_mesh_data->NumImages = (int)gltf->textures_count;
		for (int i = 0; i < p_mesh_data->NumImages; i++)
		{
			const cgltf_texture* gltf_tex = &gltf->textures[i];
			image_creation_params_t p;
			p.gltf_image_index = gltf_image_index(gltf, gltf_tex->image);
			p.min_filter = gltf_to_sg_filter(gltf_tex->sampler->min_filter);
			p.mag_filter = gltf_to_sg_filter(gltf_tex->sampler->mag_filter);
			p.wrap_s = gltf_to_sg_wrap(gltf_tex->sampler->wrap_s);
			p.wrap_t = gltf_to_sg_wrap(gltf_tex->sampler->wrap_t);
			p_mesh_data->ImageParams.Add(p);
		}

		// start loading all images
		for (cgltf_size i = 0; i < gltf->images_count; i++)
		{
			const cgltf_image* gltf_img = &gltf->images[i];
			
			for (int c = 0; c < p_mesh_data->NumImages; c++) 
			{
				image_creation_params_t* p = &p_mesh_data->ImageParams[c];
				if (p->gltf_image_index == int(i)) 
				{
					p_mesh_data->Textures.Add(gfx::LoadTexture(gltf_img->uri));
				}
			}
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
			gltf_parse_images(data, p_mesh_data);
			//gltf_parse_materials(data);
			//gltf_parse_meshes(data);
			//gltf_parse_nodes(data);
			cgltf_free(data);
		}
	}

	//------------------------------------------------------------------------------
	void Load3dModel(core::String _path, std::function<void(ComponentHandle<Node3D>)> OnModel)
	{
		io::LoadFile(_path, [](core::Buffer* buffer)
			{
				MeshData* p_mesh_data = core::Memory::New<MeshData>();
				gltf_parse((void*)buffer->Data(), buffer->Size(), p_mesh_data);
			});
	} // Load3dModel

} // namespace ari::en
