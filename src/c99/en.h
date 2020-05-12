#ifndef ARI_EN_H
#define ARI_EN_H

#include "macros.h"

// World functions
CARI_API void* CreateWorld();
CARI_API void DeleteWorld(void* _obj);
CARI_API void UpdateWorld(void* _obj, float _elapsedTime);

// System functions
CARI_API void* CreateRenderSystem();
CARI_API void DeleteSystem(void* _obj);
CARI_API void* CreateSceneSystem();

#endif // ARI_EN_H
