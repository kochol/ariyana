#pragma once
#include "Node3D.hpp"
#include "gfx/gfx.hpp"

namespace ari::en
{
	class RenderSystem;

	class BoxShape: public Node3D
	{
	public:

		ARI_COMPONENT_CHILD(BoxShape, Node3D)

		// Constructor
		BoxShape() { _isRenderable = true; }

		//! Destructor
		virtual ~BoxShape() = default;

		//! Render
		void Render(const int& _frameTurnIndex) override;

		gfx::TextureHandle Texture;

		static void Init(RenderSystem* render_system);
		static void Shutdown();

		static gfx::BufferHandle m_sVBPos;
		static gfx::BufferHandle m_sVBColor;
		static gfx::BufferHandle m_sVBTexcoord;
		static gfx::BufferHandle m_sIB;
		static gfx::ShaderHanlde m_sProgram;
		static gfx::ShaderHanlde m_sTexProgram;
		static gfx::PipelineHandle m_sPipeline;
		static gfx::PipelineHandle m_sTexPipeline;
		static gfx::BindingHandle m_sBinding;
		static gfx::BindingHandle m_sTexBinding;

	}; // BoxShape
}
