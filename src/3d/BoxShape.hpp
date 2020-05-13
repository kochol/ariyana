#pragma once
#include "Node3D.hpp"
#include "gfx/gfx.hpp"
#include "gfx/SubMesh.hpp"

namespace ari::en
{
	class RenderSystem;

	class BoxShape: public Node3D
	{
	public:

		ARI_COMPONENT_CHILD(BoxShape, Node3D)

		//! Constructor
		BoxShape();

		//! Destructor
		virtual ~BoxShape() = default;

		//! Render
		void Render(const int& _frameTurnIndex) override;

		gfx::TextureHandle Texture;
		gfx::SubMeshHandle SubMesh;

		static void Init(RenderSystem* render_system);
		static void Shutdown();

	private:

		static gfx::BufferHandle m_sVBPos;
		static gfx::BufferHandle m_sVBNormal;
		static gfx::BufferHandle m_sVBColor;
		static gfx::BufferHandle m_sVBTexcoord;
		static gfx::BufferHandle m_sIB;
		static gfx::PipelineHandle m_sPipeline;
		static gfx::PipelineHandle m_sTexPipeline;
		static gfx::BindingHandle m_sBinding;
		static gfx::BindingHandle m_sTexBinding;

		gfx::SubMesh* m_pSubMesh = nullptr;

	}; // BoxShape
}

#include "Meta.h"

namespace meta
{
	template <>
	inline auto registerMembers<ari::en::BoxShape>()
	{
		return std::tuple_cat(
			meta::getMembers<ari::en::Node3D>()
		);
	}
}
