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

		enum class ShaderType
		{
			Basic = 0,
			BasicTexture,

			Count
		};

		bool SetupGfx(GfxSetup& setup);
		
		void SetupShaders();

		ShaderHanlde GetShader(ShaderType shader);

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

		TextureHandle CreateTexture(core::String _path);
        
    } // namespace gfx
    
} // namespace ari
