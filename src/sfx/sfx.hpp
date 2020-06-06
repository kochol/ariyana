#pragma once

#include "en/Node.hpp"

namespace ari::sfx
{
	class Sfx : public Node
	{
	public:
		ARI_COMPONENT(Sfx)

		virtual ~Sfx() = default;
	};

} // namespace ari::sfx
