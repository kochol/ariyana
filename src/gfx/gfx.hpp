#pragma once
#include "io/Window.hpp"

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

		struct PipelineSetup
		{
			LayoutSetup layout;
			ShaderHanlde shader;
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

		bool SetupGfx(GfxSetup& setup);

		void Present();

		void Present(const io::WindowHandle& handle);

		uint32_t GetFrameNumber();

		BufferHandle CreateVertexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		BufferHandle CreateIndexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		ShaderHanlde CreateShader(const sg_shader_desc* desc);

		PipelineHandle CreatePipeline(const PipelineSetup& setup);

		void ApplyPipeline(PipelineHandle pipeline);

		BindingHandle CreateBinding(const Bindings& bindings);

		void ApplyBindings(const BindingHandle& handle);

		void BeginDefaultPass();

		void EndPass();

		void Commit();

		void Draw(int base_element, int num_elements, int num_instances);
        
    } // namespace gfx
    
} // namespace ari
