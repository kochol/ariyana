#pragma once
#include "io/Window.hpp"
#include "sx/math.h"
#include "core/string/String.hpp"

struct sg_shader_desc;
#define ARI_MAX_SHADERSTAGE_TEXTURES 12
#define ARI_MAX_SHADERSTAGE_BUFFERS 8
#define ARI_MAX_VERTEX_ATTRIBUTES 16

namespace ari
{
    namespace gfx
    {
		ARI_HANDLE(BufferHandle);
		ARI_HANDLE(ShaderHandle);
		ARI_HANDLE(PipelineHandle);
		ARI_HANDLE(TextureHandle);
		ARI_HANDLE(BindingHandle);		

        struct GfxSetup
        {
			io::Window	window;
			int sample_count = 0;                   /* MSAA sample count */
			int swap_interval = 0;                  /* the preferred swap interval (ignored on some platforms) */
		};

		enum class BufferUsage
		{
			Immutable = 1,
			Dynamic,
			Stream
		};

		enum class VertexStep
		{
			PerVertex = 1,
			PerInstance
		};

		struct BufferLayoutSetup
		{
			int stride = 0;
			VertexStep step = VertexStep::PerVertex;
			int stepRate = 0;
		};

		enum class VertexFormat
		{
			Invalid,
			Float,
			Float2,
			Float3,
			Float4,
			Byte4,
			Byte4N,
			UByte4,
			UByte4N,
			Short2,
			Short2N,
			Short4,
			Short4N,
			UInt10N2
		};

		struct VertexAttrSetup
		{
			int bufferIndex = 0;
			int offset = 0;
			VertexFormat format = VertexFormat::Invalid;
		};

		struct LayoutSetup
		{
			BufferLayoutSetup buffers[ARI_MAX_SHADERSTAGE_BUFFERS];
			VertexAttrSetup attrs[ARI_MAX_VERTEX_ATTRIBUTES];
		};

		enum class IndexType
		{
			None = 1,
			Uint16,
			Uint32
		};

		/*
			BlendFactor

			The source and destination factors in blending operations.
			This is used in the following members when creating a pipeline object:

			PipelineSetup
				.blend
					.src_factor_rgb
					.dst_factor_rgb
					.src_factor_alpha
					.dst_factor_alpha

			The default value is One for source
			factors, and Zero for destination factors.
		*/
		enum class BlendFactor 
    	{
			Default,    /* value 0 reserved for default-init */
			Zero,
			One,
			Src_Color,
			One_Minus_Src_Color,
			Src_Alpha,
			One_Minus_Src_Alpha,
			Dst_Color,
			One_Minus_Dst_Color,
			Dst_Alpha,
			One_Minus_Dst_Alpha,
			Src_Alpha_Saturated,
			Blend_Color,
			One_Minus_Blend_Color,
			Blend_Alpha,
			One_Minus_Blend_Alpha,
		};

		/*
			BlendOp

			Describes how the source and destination values are combined in the
			fragment blending operation. It is used in the following members when
			creating a pipeline object:

			sg_pipeline_desc
				.blend
					.op_rgb
					.op_alpha

			The default value is Add.
		*/
		enum class BlendOp
		{
			Default,    /* value 0 reserved for default-init */
			Add,
			Subtract,
			Reverse_Subtract,
		};

		/*
			PixelFormat

			sokol_gfx.h basically uses the same pixel formats as WebGPU, since these
			are supported on most newer GPUs. GLES2 and WebGL has a much smaller
			subset of available pixel formats. Call sg_query_pixelformat() to check
			at runtime if a pixel format supports the desired features.

			A pixelformat name consist of three parts:

				- components (R, RG, RGB or RGBA)
				- bit width per component (8, 16 or 32)
				- component data type:
					- unsigned normalized (no postfix)
					- signed normalized (SN postfix)
					- unsigned integer (UI postfix)
					- signed integer (SI postfix)
					- float (F postfix)

			Not all pixel formats can be used for everything, call sg_query_pixelformat()
			to inspect the capabilities of a given pixelformat. The function returns
			an sg_pixelformat_info struct with the following bool members:

				- sample: the pixelformat can be sampled as texture at least with
						  nearest filtering
				- filter: the pixelformat can be samples as texture with linear
						  filtering
				- render: the pixelformat can be used for render targets
				- blend:  blending is supported when using the pixelformat for
						  render targets
				- msaa:   multisample-antialiasing is supported when using the
						  pixelformat for render targets
				- depth:  the pixelformat can be used for depth-stencil attachments

			When targeting GLES2/WebGL, the only safe formats to use
			as texture are SG_PIXELFORMAT_R8 and SG_PIXELFORMAT_RGBA8. For rendering
			in GLES2/WebGL, only SG_PIXELFORMAT_RGBA8 is safe. All other formats
			must be checked via sg_query_pixelformats().

			The default pixel format for texture images is SG_PIXELFORMAT_RGBA8.

			The default pixel format for render target images is platform-dependent:
				- for Metal and D3D11 it is SG_PIXELFORMAT_BGRA8
				- for GL backends it is SG_PIXELFORMAT_RGBA8

			This is mainly because of the default framebuffer which is setup outside
			of sokol_gfx.h. On some backends, using BGRA for the default frame buffer
			allows more efficient frame flips. For your own offscreen-render-targets,
			use whatever renderable pixel format is convenient for you.
		*/
		enum class PixelFormat 
    	{
			Default,    /* value 0 reserved for default-init */
			None,

			R8,
			R8SN,
			R8UI,
			R8SI,

			R16,
			R16SN,
			R16UI,
			R16SI,
			R16F,
			RG8,
			RG8SN,
			RG8UI,
			RG8SI,

			R32UI,
			R32SI,
			R32F,
			RG16,
			RG16SN,
			RG16UI,
			RG16SI,
			RG16F,
			RGBA8,
			RGBA8SN,
			RGBA8UI,
			RGBA8SI,
			BGRA8,
			RGB10A2,
			RG11B10F,

			RG32UI,
			RG32SI,
			RG32F,
			RGBA16,
			RGBA16SN,
			RGBA16UI,
			RGBA16SI,
			RGBA16F,

			RGBA32UI,
			RGBA32SI,
			RGBA32F,

			DEPTH,
			DEPTH_STENCIL,

			BC1_RGBA,
			BC2_RGBA,
			BC3_RGBA,
			BC4_R,
			BC4_RSN,
			BC5_RG,
			BC5_RGSN,
			BC6H_RGBF,
			BC6H_RGBUF,
			BC7_RGBA,
			PVRTC_RGB_2BPP,
			PVRTC_RGB_4BPP,
			PVRTC_RGBA_2BPP,
			PVRTC_RGBA_4BPP,
			ETC2_RGB8,
			ETC2_RGB8A1,
			ETC2_RGBA8,
			ETC2_RG11,
			ETC2_RG11SN,
		};

		struct BlendState {
			bool enabled = false;
			BlendFactor src_factor_rgb = BlendFactor::Default;
			BlendFactor dst_factor_rgb = BlendFactor::Default;
			BlendOp op_rgb = BlendOp::Default;
			BlendFactor src_factor_alpha = BlendFactor::Default;
			BlendFactor dst_factor_alpha = BlendFactor::Default;
			BlendOp op_alpha = BlendOp::Default;
			uint8_t color_write_mask = 0;
			int color_attachment_count = 0;
			PixelFormat color_format = PixelFormat::Default;
			PixelFormat depth_format = PixelFormat::Default;
			float blend_color[4];

			/// equality operator
			bool operator==(const BlendState& rhs) const
			{
				// quick check
				if (!enabled && !rhs.enabled)
					return true;

				if (enabled != rhs.enabled
					|| src_factor_rgb != rhs.src_factor_rgb
					|| dst_factor_rgb != rhs.dst_factor_rgb
					|| op_rgb != rhs.op_rgb
					|| src_factor_alpha != rhs.src_factor_alpha
					|| dst_factor_alpha != rhs.dst_factor_alpha
					|| op_alpha != rhs.op_alpha
					|| color_write_mask != rhs.color_write_mask
					|| color_attachment_count != rhs.color_attachment_count
					|| color_format != rhs.color_format
					|| depth_format != rhs.depth_format)
					return false;
				return sx_equal_arr(blend_color, rhs.blend_color, 4, 0.0001f);
			}
		};

		struct PipelineSetup
		{
			LayoutSetup layout;
			ShaderHandle shader;
			IndexType index_type = IndexType::None;
			BlendState blend;

			/// equality operator
			bool operator==(const PipelineSetup& rhs) const
			{
				if (shader.Handle != rhs.shader.Handle
					|| index_type != rhs.index_type)
					return false;
				for (int i = 0; i < ARI_MAX_SHADERSTAGE_BUFFERS; ++i)
				{
					if (layout.buffers[i].step != rhs.layout.buffers[i].step
						|| layout.buffers[i].stepRate != rhs.layout.buffers[i].stepRate
						|| layout.buffers[i].stride != rhs.layout.buffers[i].stride)
						return false;
				}
				for (int i = 0; i < ARI_MAX_VERTEX_ATTRIBUTES; ++i)
				{
					if (layout.attrs[i].bufferIndex != rhs.layout.attrs[i].bufferIndex
						|| layout.attrs[i].offset != rhs.layout.attrs[i].offset
						|| layout.attrs[i].format != rhs.layout.attrs[i].format)
						return false;
				}
				return blend == rhs.blend;
			}
		};

		struct Bindings
		{
			BufferHandle vertexBuffers[ARI_MAX_SHADERSTAGE_BUFFERS];
			int vertexBufferOffsets[ARI_MAX_SHADERSTAGE_BUFFERS] = { 0,0,0,0,0,0,0,0 };
			BufferHandle indexBuffer;
			int indexBufferOffset = 0;
			TextureHandle vsTextures[ARI_MAX_SHADERSTAGE_TEXTURES];
			TextureHandle fsTextures[ARI_MAX_SHADERSTAGE_TEXTURES];

			/// equality operator
			bool operator==(const Bindings& rhs) const
			{
				if (indexBuffer.Handle != rhs.indexBuffer.Handle
					|| indexBufferOffset != rhs.indexBufferOffset)
					return false;
				for (int i = 0; i < ARI_MAX_SHADERSTAGE_BUFFERS; ++i)
				{
					if (vertexBufferOffsets[i] != rhs.vertexBufferOffsets[i]
						|| vertexBuffers[i].Handle != rhs.vertexBuffers[i].Handle)
						return false;
				}
				for (int i = 0; i < ARI_MAX_SHADERSTAGE_TEXTURES; ++i)
				{
					if (fsTextures[i].Handle != rhs.fsTextures[i].Handle
						|| vsTextures[i].Handle != rhs.vsTextures[i].Handle)
						return false;
				}
				return true;
			}
		};

		enum class ShaderStage
		{
			VertexShader,
			FragmentShader
		};

		/*
			TextureFilter

			The filtering mode when sampling a texture image. This is
			used in the sg_image_desc.min_filter and sg_image_desc.mag_filter
			members when creating an image object.

			The default filter mode is SG_FILTER_NEAREST.
		*/
		enum class TextureFilter 
    	{
			_Default, /* value 0 reserved for default-init */
			Nearest,
			Linear,
			NearestMipmapNearest,
			NearestMipmapLinear,
			LinearMipmapNearest,
			LinearMipmapLinear,
			_Num,
			_FilterForceU32 = 0x7FFFFFFF
		};

		/*
			sg_wrap

			The texture coordinates wrapping mode when sampling a texture
			image. This is used in the sg_image_desc.wrap_u, .wrap_v
			and .wrap_w members when creating an image.

			The default wrap mode is SG_WRAP_REPEAT.

			NOTE: SG_WRAP_CLAMP_TO_BORDER is not supported on all backends
			and platforms. To check for support, call sg_query_features()
			and check the "clamp_to_border" boolean in the returned
			sg_features struct.

			Platforms which don't support SG_WRAP_CLAMP_TO_BORDER will silently fall back
			to SG_WRAP_CLAMP_TO_EDGE without a validation error.

			Platforms which support clamp-to-border are:

				- all desktop GL platforms
				- Metal on macOS
				- D3D11

			Platforms which do not support clamp-to-border:

				- GLES2/3 and WebGL/WebGL2
				- Metal on iOS
		*/
		enum class TextureWrap 
    	{
			_Default,   /* value 0 reserved for default-init */
			Repeat,
			ClampToEdge,
			ClampToBorder,
			MirroredRepeat,
			_NUM,
			_FORCEU32 = 0x7FFFFFFF
		};

		/*
			TextureBorderColor

			The border color to use when sampling a texture, and the UV wrap
			mode is ClampToBorder.

			The default border color is OpaqueBlack
		*/
		enum class TextureBorderColor 
    	{
			Default,    /* value 0 reserved for default-init */
			TransparentBlack,
			OpaqueBlack,
			OpaqueWhite,
			Num,
			ForceU32 = 0x7FFFFFFF
		};

		enum class PrimitiveType
		{
			Points,
			Lines,
			LineLoop,
			LineStrip,
			Triangles,
			TrianglesStrip,
			TrianglesFan,
		};
		
		enum class ShaderType
		{
			Basic = 0,
			BasicTexture,
			BasicVertexColor,

			Count
		};

		struct Material;

		bool SetupGfx(GfxSetup& setup);
		
		void SetupShaders();

		ShaderHandle GetShader(ShaderType shader);

		void SetMaterialShader(Material& material);

		void RenderToWindow(const io::WindowHandle& handle);

		void Present(const io::WindowHandle& handle = io::MainWindow);

		uint32_t GetFrameNumber();

		BufferHandle CreateVertexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		BufferHandle CreateIndexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		void DestroyBuffer(BufferHandle& buffer);

		ShaderHandle CreateShader(const sg_shader_desc* desc);

		void DestroyShader(ShaderHandle& shader);

		PipelineHandle CreatePipeline(const PipelineSetup& setup);

		void DestroyPipeline(PipelineHandle& pipeline);

		void ApplyPipeline(const PipelineHandle& pipeline);

		void ApplyPipelineAndMaterial(PipelineHandle& pipeline, Material* material);

		void SetPipelineBlendState(PipelineHandle& pipeline, const BlendState& blend);
    	
    	void SetPipelineShader(PipelineHandle& pipeline, const ShaderHandle& shader);

		BindingHandle CreateBinding(const Bindings& bindings);

		void DestroyBinding(BindingHandle& binding);

		void ApplyBindings(const BindingHandle& handle);

		void SetTexture(const BindingHandle& handle, int stage, TextureHandle texture);

		void ApplyUniforms(ShaderStage _stage, int _ub_index, const void* _data, int _size);

		void BeginDefaultPass(const io::WindowHandle& _handle = io::MainWindow);

		void EndPass();

		void Commit();

		void Draw(int base_element, int num_elements, int num_instances);

		void SetViewMatrix(const sx_mat4& _view);

		sx_mat4 GetViewMatrix();

		void SetProjMatrix(const sx_mat4& _proj);

		sx_mat4 GetProjMatrix();

		/// Set world after view and projection matrix
		void SetWorldMatrix(const sx_mat4& _world);

		sx_mat4 GetWorldMatrix();

		void SetViewProjMatrix(const sx_mat4& _view, const sx_mat4& _proj);

		sx_mat4 GetViewProjMatrix();

		void SetWorldViewProjMatrix(const sx_mat4& _world, const sx_mat4& _view, const sx_mat4& _proj);

		sx_mat4 GetWorldViewProjMatrix();

    	TextureHandle LoadTexture(core::String _path);

		void SetDirLight(const sx_vec3& dir, const sx_vec4& color);

		void SetOmniLight(const sx_vec3& pos, const sx_vec4& color);

		void SetCameraPosition(const sx_vec3& pos);
        
    } // namespace gfx
    
} // namespace ari
