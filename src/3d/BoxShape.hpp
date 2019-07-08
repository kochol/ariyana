#pragma once
#include "Node3D.hpp"
#include "gfx/gfx.hpp"

namespace ari::en
{
	class RenderSystem;

	class BoxShape: public Node3D
	{
	public:

		// Constructor
		BoxShape() { _isRenderable = true; }

		//! Destructor
		virtual ~BoxShape() = default;

		//! Render
		virtual void Render(const int& _frameTurnIndex) override;

		static void Init(RenderSystem* render_system);
		static void Shutdown();

		static gfx::BufferHandle m_sVBPos;
		static gfx::BufferHandle m_sVBColor;
		static gfx::BufferHandle m_sIB;
		static gfx::ShaderHanlde m_sProgram;
		static gfx::PipelineHandle m_sPipeline;
		static gfx::BindingHandle m_sBinding;

	}; // BoxShape
}
