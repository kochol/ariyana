#pragma once

#include "soloud_wav.h"

#include "core/defines.hpp"

namespace ari::sfx
{
	// channel handle can be use 
	ARI_HANDLE(ChannelHandle);

	//! Initialize Sfx
	void InitSfx();

	//! Clean up Sfx
	void ShutdownSfx();

	ChannelHandle Play(SoLoud::Wav& _wave);
	
} // namespace ari::sfx
