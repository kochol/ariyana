#pragma once

#include "Node2D.hpp"
#include "core/defines.hpp"

namespace ari::en
{
	class CanvasNode : public Node2D
	{
	public:
		ARI_COMPONENT(CanvasNode)

		//! Constructor
		CanvasNode()
		{
		}

		//! Destructor
		virtual ~CanvasNode() = default;

		sx_rect Size;

	};
} // ari::en
