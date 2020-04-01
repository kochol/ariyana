#include "3dModel.hpp"
#include "io/FileSystem.hpp"
#include "private/cgltf/cgltf.h"
#include <sokol_gfx.h>
#include "gfx/gfx.hpp"
#include "gfx/Mesh.hpp"
#include "private/gltf.hpp"

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
												NumImages,
												NumMaterials,
												NumMeshes;
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
	// parse GLTF materials into our own material definition
	//------------------------------------------------------------------------------
	static void gltf_parse_materials(const cgltf_data* gltf, MeshData* p_mesh_data)
	{
		p_mesh_data->NumMaterials = int(gltf->materials_count);
		// TODO: Convert gltf material to our material system.
		/*for (int i = 0; i < p_mesh_data->NumMaterials; i++) 
		{
			const cgltf_material* gltf_mat = &gltf->materials[i];
			material_t* scene_mat = &state.scene.materials[i];
			scene_mat->is_metallic = gltf_mat->has_pbr_metallic_roughness;
			if (scene_mat->is_metallic) {
				const cgltf_pbr_metallic_roughness* src = &gltf_mat->pbr_metallic_roughness;
				metallic_material_t* dst = &scene_mat->metallic;
				for (int d = 0; d < 4; d++) {
					dst->fs_params.base_color_factor.Elements[d] = src->base_color_factor[d];
				}
				for (int d = 0; d < 3; d++) {
					dst->fs_params.emissive_factor.Elements[d] = gltf_mat->emissive_factor[d];
				}
				dst->fs_params.metallic_factor = src->metallic_factor;
				dst->fs_params.roughness_factor = src->roughness_factor;
				dst->images = (metallic_images_t){
					.base_color = gltf_texture_index(gltf, src->base_color_texture.texture),
					.metallic_roughness = gltf_texture_index(gltf, src->metallic_roughness_texture.texture),
					.normal = gltf_texture_index(gltf, gltf_mat->normal_texture.texture),
					.occlusion = gltf_texture_index(gltf, gltf_mat->occlusion_texture.texture),
					.emissive = gltf_texture_index(gltf, gltf_mat->emissive_texture.texture)
				};
			}
			else {
				*//*
				const cgltf_pbr_specular_glossiness* src = &gltf_mat->pbr_specular_glossiness;
				specular_material_t* dst = &scene_mat->specular;
				for (int d = 0; d < 4; d++) {
					dst->fs_params.diffuse_factor.Elements[d] = src->diffuse_factor[d];
				}
				for (int d = 0; d < 3; d++) {
					dst->fs_params.specular_factor.Elements[d] = src->specular_factor[d];
				}
				for (int d = 0; d < 3; d++) {
					dst->fs_params.emissive_factor.Elements[d] = gltf_mat->emissive_factor[d];
				}
				dst->fs_params.glossiness_factor = src->glossiness_factor;
				dst->images = (specular_images_t) {
					.diffuse = gltf_texture_index(gltf, src->diffuse_texture.texture),
					.specular_glossiness = gltf_texture_index(gltf, src->specular_glossiness_texture.texture),
					.normal = gltf_texture_index(gltf, gltf_mat->normal_texture.texture),
					.occlusion = gltf_texture_index(gltf, gltf_mat->occlusion_texture.texture),
					.emissive = gltf_texture_index(gltf, gltf_mat->emissive_texture.texture)
				};
				
			}
		}*/
	}

	//------------------------------------------------------------------------------
	// parse GLTF meshes into our own mesh and submesh definition
	//------------------------------------------------------------------------------
	static void gltf_parse_meshes(const cgltf_data* gltf, MeshData* p_mesh_data)
	{
		core::ObjectPool<gfx::Mesh>::Setup(64);
		core::ObjectPool<gfx::SubMesh>::Setup(128);

		p_mesh_data->NumMeshes = int(gltf->meshes_count);
		for (cgltf_size mesh_index = 0; mesh_index < gltf->meshes_count; mesh_index++) 
		{
			const cgltf_mesh* gltf_mesh = &gltf->meshes[mesh_index];

			gfx::MeshHandle mesh_handle;
			mesh_handle.Handle = core::HandleManager<gfx::Mesh>::GetNewHandle(mesh_handle.Index);
			auto mesh = core::ObjectPool<gfx::Mesh>::New(mesh_handle.Index);
			mesh->SubMeshes.Reserve(int(gltf_mesh->primitives_count));

			for (cgltf_size prim_index = 0; prim_index < gltf_mesh->primitives_count; prim_index++) 
			{
				const cgltf_primitive* gltf_prim = &gltf_mesh->primitives[prim_index];
				gfx::SubMeshHandle sub_mesh_handle;
				sub_mesh_handle.Handle = core::HandleManager<gfx::SubMesh>::GetNewHandle(sub_mesh_handle.Index);
				auto sub_mesh = core::ObjectPool<gfx::SubMesh>::New(sub_mesh_handle.Index);

				/*// a mapping from sokol-gfx vertex buffer bind slots into the scene.buffers array
				prim->vertex_buffers = create_vertex_buffer_mapping_for_gltf_primitive(gltf, gltf_prim);
				// create or reuse a matching pipeline state object
				prim->pipeline = create_sg_pipeline_for_gltf_primitive(gltf, gltf_prim, &prim->vertex_buffers);
				// the material parameters
				prim->material = gltf_material_index(gltf, gltf_prim->material);
				// index buffer, base element, num elements
				if (gltf_prim->indices) {
					prim->index_buffer = gltf_bufferview_index(gltf, gltf_prim->indices->buffer_view);
					assert(state.creation_params.buffers[prim->index_buffer].type == SG_BUFFERTYPE_INDEXBUFFER);
					assert(gltf_prim->indices->stride != 0);
					prim->base_element = 0;
					prim->num_elements = (int)gltf_prim->indices->count;
				}
				else {
					// hmm... looking up the number of elements to render from
					// a random vertex component accessor looks a bit shady
					prim->index_buffer = SCENE_INVALID_INDEX;
					prim->base_element = 0;
					prim->num_elements = (int)gltf_prim->attributes->data->count;
				}*/
			}
		}
	}

	// parse GLTF nodes into our own node definition
	static void gltf_parse_nodes(const cgltf_data* gltf) 
	{
		/*
		if (gltf->nodes_count > SCENE_MAX_NODES) {
			state.failed = true;
			return;
		}
		for (cgltf_size node_index = 0; node_index < gltf->nodes_count; node_index++) {
			const cgltf_node* gltf_node = &gltf->nodes[node_index];
			// ignore nodes without mesh, those are not relevant since we
			// bake the transform hierarchy into per-node world space transforms
			if (gltf_node->mesh) {
				node_t* node = &state.scene.nodes[state.scene.num_nodes++];
				node->mesh = gltf_mesh_index(gltf, gltf_node->mesh);
				node->transform = build_transform_for_gltf_node(gltf, gltf_node);
			}
		}*/
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
			gltf_parse_materials(data, p_mesh_data);
			gltf_parse_meshes(data, p_mesh_data);
			gltf_parse_nodes(data);
			cgltf_free(data);
		}
	}

	void Load3dModel(const core::String& _path, const std::function<void(ComponentHandle<Node3D>)>& OnModel)
	{
		io::LoadFile(_path, [](core::Buffer* buffer)
			{
				MeshData* p_mesh_data = core::Memory::New<MeshData>();
				gltf_parse((void*)buffer->Data(), buffer->Size(), p_mesh_data);
			});
	} // Load3dModel

	//------------------------------------------------------------------------------
	void Load3dScene(const core::String& _path, std::function<void(core::Array<ComponentHandle<Node3D>>)> OnModel)
	{
		LoadGltfScene(_path, std::move(OnModel));
	}

} // namespace ari::en
