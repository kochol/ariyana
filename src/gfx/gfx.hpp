#pragma once
#include "io/Window.hpp"
#include "sx/math.h"
#include "core/string/String.hpp"

struct sg_shader_desc;
#define ARI_MAX_SHADERSTAGE_TEXTURES 12
#define ARI_MAX_SHADERSTAGE_BUFFERS 4
#define ARI_MAX_VERTEX_ATTRIBUTES 16

namespace ari
{
    namespace gfx
    {
		ARI_HANDLE(BufferHandle);
		ARI_HANDLE(ShaderHanlde);
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

		struct PipelineSetup
		{
			LayoutSetup layout;
			ShaderHanlde shader;
			IndexType index_type = IndexType::None;
		};

		struct Bindings
		{
			BufferHandle vertexBuffers[ARI_MAX_SHADERSTAGE_BUFFERS];
			int vertexBufferOffsets[ARI_MAX_SHADERSTAGE_BUFFERS] = { 0,0,0,0 };
			BufferHandle indexBuffer;
			int indexBufferOffset = 0;
			TextureHandle vsTextures[ARI_MAX_SHADERSTAGE_TEXTURES];
			TextureHandle fsTextures[ARI_MAX_SHADERSTAGE_TEXTURES];
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

		bool SetupGfx(GfxSetup& setup);

		void RenderToWindow(const io::WindowHandle& handle);

		void Present(const io::WindowHandle& handle = io::MainWindow);

		uint32_t GetFrameNumber();

		BufferHandle CreateVertexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		BufferHandle CreateIndexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		void DestroyBuffer(BufferHandle& buffer);

		ShaderHanlde CreateShader(const sg_shader_desc* desc);

		void DestroyShader(ShaderHanlde& shader);

		PipelineHandle CreatePipeline(const PipelineSetup& setup);

		void DestroyPipeline(PipelineHandle& pipeline);

		void ApplyPipeline(const PipelineHandle& pipeline);

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

		void SetViewProjMatrix(const sx_mat4& _view, const sx_mat4& _proj);

		sx_mat4 GetViewProjMatrix();

		TextureHandle LoadTexture(core::String _path);
        
    } // namespace gfx
    
} // namespace ari
