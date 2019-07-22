#pragma once
#include "en/Node.hpp"

namespace ari
{
	namespace gui
	{
		class Gui: public en::Node
		{
		public:

			ARI_COMPONENT

			virtual ~Gui() = default;

			virtual bool BeginRender() { return true; }

			virtual void EndRender() { }

			bool SameLine = false;

			bool Separator = false;

			bool Visible = true;

		}; // Gui

	}

} // ari
