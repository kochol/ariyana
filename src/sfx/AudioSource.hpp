#pragma once

#include "sfx.hpp"
#include "en/World.hpp"

namespace ari::sfx
{
	class AudioSource : public Sfx
	{
	public:
		ARI_COMPONENT_CHILD(AudioSource, Sfx)
	};

} // namespace ari::sfx
