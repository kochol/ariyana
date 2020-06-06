#pragma once

#include "en/System.hpp"
#include "sfx.hpp"
#include "soloud.h"

namespace ari::sfx
{
	class SfxSystem : public en::System
	{
	public:

		SfxSystem();

		~SfxSystem();

		void Configure(World* _world) override;

		void Unconfigure(World* _world) override;

	};

} // namespace ari::sfx
