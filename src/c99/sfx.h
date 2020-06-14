#ifndef ARI_SFX_H
#define ARI_SFX_H

#include "macros.h"

// Globals
CARI_API void InitSfx();

CARI_API void ShutdownSfx();

CARI_HANDLE(ChannelHandle)
CARI_API ChannelHandle Play(void* _audio);

CARI_HANDLE(AudioSourceHandle)
CARI_API AudioSourceHandle LoadAudioSource(char* _path);

#endif // ARI_SFX_H
