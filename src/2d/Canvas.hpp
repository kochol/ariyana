#pragma once

#include "CanvasNode.hpp"
#include "sx/math.h"

namespace ari::en
{
	class Canvas : public CanvasNode
	{
	public:
		ARI_COMPONENT_CHILD(Canvas, CanvasNode)

		//! Constructor
		Canvas()
		{
		}

		//! Destructor
		virtual ~Canvas() = default;

		void SetViewportRect();
	};

} // ari::en
