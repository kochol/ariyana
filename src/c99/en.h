#ifndef ARI_EN_H
#define ARI_EN_H

#include "macros.h"
#include "core.h"
#include "gfx.h"

struct EntityHandle
{
	uint32_t Handle;
	uint32_t Index;
	void* Entity;
};

// World functions
CARI_API void* CreateWorld();
CARI_API void DeleteWorld(void* _obj);
CARI_API void UpdateWorld(void* _obj, float _elapsedTime);
CARI_API void AddSystemToWorld(void* _world, void* _system);
CARI_API void AddEntityToWorld(void* _world, EntityHandle* _entity);

// System functions
CARI_API void AddChildSystem(void* _obj, void* _system);
CARI_API void DeleteSystem(void* _obj);

// Entity functions
CARI_API EntityHandle CreateEntityWorld();
CARI_API void DeleteEntityWorld(EntityHandle* _entity);
CARI_API bool IsValidEntity(uint32_t& _entityHandle);
CARI_API bool* GetEntityMembers(EntityHandle* _entity);

// Node
CARI_API const char* GetNodeName(void* _node);
CARI_API void SetNodeName(void* _node, char* _name);
CARI_API void NodeAddChild(void* _node, void* _child);
CARI_API void NodeRemoveChild(void* _node, void* _child);

#endif // ARI_EN_H
