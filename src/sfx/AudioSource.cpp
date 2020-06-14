#pragma once

#include "sfx.hpp"
#include"AudioSource.hpp"

namespace ari::sfx
{
	SoLoud::Wav wave;
		

	AudioSourceHandle LoadAudioSource(char* _path)
	{
		ari::sfx::
		wave.load(_path);
		return { core::HandleManager<AudioSourceHandle>::CreateHandleByIndex(wave.mAudioSourceID), wave.mAudioSourceID };
	}


} // namespace ari::sfx
