#include "sfx.h"

#include "sfx/sfx.hpp"

// Globals
void InitSfx()
{
	ari::sfx::InitSfx();
}

void ShutdownSfx()
{
	ari::sfx::ShutdownSfx();
}

bool IsValidChannel(uint32_t& _handle)
{
	return ari::core::HandleManager<ari::sfx::AudioSourceHandle>::IsHandleValid(_handle);
}

ChannelHandle Play(void* _audio)
{
	union 
	{ 
		ari::sfx::ChannelHandle cpp; 
		ChannelHandle c{};
	} r = { ari::sfx::Play(*(SoLoud::Wav*)_audio) };

	return r.c;
}

// AudioSource
bool IsValidAudio(uint32_t& _handle)
{
	return ari::core::HandleManager<ari::sfx::AudioSourceHandle>::IsHandleValid(_handle);
}


AudioSourceHandle LoadAudioSource(char* _path)
{
	union
	{
		ari::sfx::AudioSourceHandle cpp;
		AudioSourceHandle c{};
	} r = { ari::sfx::LoadAudioSource(_path) };

	return r.c;
}
