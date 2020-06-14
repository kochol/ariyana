#include "sfx.h"

#include "sfx/sfx.hpp"
#include "sfx/AudioSource.hpp"

// Globals

void InitSfx()
{
	ari::sfx::InitSfx();
}

void ShutdownSfx()
{
	ari::sfx::ShutdownSfx();
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
//
AudioSourceHandle LoadAudioSource(char* _path)
{
	union
	{
		ari::sfx::AudioSourceHandle cpp;
		AudioSourceHandle c{};
	} r = { ari::sfx::LoadAudioSource(_path) };

	return r.c;
}
