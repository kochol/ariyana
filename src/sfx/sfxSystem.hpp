#pragma once

#include "en/System.hpp"
#include "sfx.hpp"
#include "soloud.h"

namespace ari::en
{
	namespace sfx
	{
		class SfxSystem : public en::System
		{
		public:

			SfxSystem();

			~SfxSystem();

			void Configure(World* _world) override;

			void Unconfigure(World* _world) override;

		};

	} // namespace sfx

} // namespace ari
