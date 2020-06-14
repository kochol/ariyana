#pragma once

#include "core/defines.hpp"

namespace ari::sfx
{
	ARI_HANDLE(AudioSourceHandle);

	AudioSourceHandle LoadAudioSource(char* _path);
		
} // namespace ari::sfx
