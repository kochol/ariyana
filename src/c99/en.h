#ifndef ARI_EN_H
#define ARI_EN_H

#include "macros.h"

// World functions
CARI_API void* CreateWorld();
CARI_API void DeleteWorld(void* _obj);
CARI_API void UpdateWorld(void* _obj, float _elapsedTime);
CARI_API void AddSystemToWorld(void* _world, void* _system);

// System functions
CARI_API void* CreateRenderSystem();
CARI_API void DeleteSystem(void* _obj);
CARI_API void* CreateSceneSystem();

// Entity functions
struct EntityHandle
{
	uint32_t Handle;
	uint32_t Index;
};
CARI_API EntityHandle CreateEntityWorld();
CARI_API bool IsValidEntity(uint32_t& _entityHandle);

#endif // ARI_EN_H
