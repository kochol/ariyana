#pragma once

#include "AudioSource.hpp"

namespace ari::sfx
{
	
	//! Initialize Sfx
	void InitSfx();

	//! Clean up Sfx
	void ShutdownSfx();

	int Play(AudioSource* audioSource);
} // namespace ari::sfx
