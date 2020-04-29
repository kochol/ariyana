#pragma once
#include "Node2D.hpp"
#include "gfx/gfx.hpp"

namespace ari::en
{
	class RenderSystem2D;

	class Sprite : public Node2D
	{
	public:

		ARI_COMPONENT_CHILD(Sprite, Node2D)

		//! Constructor
		Sprite() { _isRenderable = true; }

		//! Destructor
		virtual ~Sprite() = default;

		//! Render
		void Render(const int& _frameTurnIndex) override;

		gfx::TextureHandle Texture;

		static void Init(RenderSystem2D* render_system_2D);
		static void Shutdown();
	};

} // ari::en


#include "Meta.h"

namespace meta
{
	template <>
	inline auto registerMembers<ari::en::Sprite>()
	{
		return std::tuple_cat(
			meta::getMembers<ari::en::Node2D>()
		);
	}
}
