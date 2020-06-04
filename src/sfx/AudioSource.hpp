#pragma once

#include "sfx.hpp"

namespace ari::en
{
	namespace sfx
	{
		class AudioSource : public Sfx
		{
		public:
			ARI_COMPONENT_CHILD(AudioSource, Sfx)
		};
	} // namespace sfx

} // namespace ari
