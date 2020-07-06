#pragma once

#include "soloud_wav.h"

#include "core/defines.hpp"
#include "core/string/String.hpp"

namespace ari::sfx
{
	// channel handle can be use 
	ARI_HANDLE(ChannelHandle);
	ARI_HANDLE(AudioSourceHandle);


	//! Initialize Sfx
	void InitSfx();

	//! Clean up Sfx
	void ShutdownSfx();

	//! Play a sound
	ChannelHandle Play(SoLoud::Wav& _wave);

	//! Stop a sound
	void Stop(ChannelHandle& _channelHandle);

	AudioSourceHandle LoadAudioSource(core::String _path);

} // namespace ari::sfx
