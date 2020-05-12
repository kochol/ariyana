#ifndef ARI_EN_H
#define ARI_EN_H

#include "macros.h"
#include "core.h"

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
CARI_HANDLE(EntityHandle)
CARI_API EntityHandle CreateEntityWorld();
CARI_API bool IsValidEntity(uint32_t& _entityHandle);

// Node3D
CARI_COMPONENT_HANDLE(Node3dHandle)
CARI_API bool IsValidNode3D(uint32_t& _cameraHandle);
CARI_API Vector3* GetNode3dPosition(void* _node);

// Camera
CARI_API Node3dHandle CreateCameraComponent();
CARI_API void AddCameraToWorld(void* _world, EntityHandle* _entity, const Node3dHandle& _camera);
CARI_API Vector3* GetCameraTarget(void* _node);

// BoxShape
CARI_API Node3dHandle CreateBoxShapeComponent();
CARI_API void AddBoxShapeToWorld(void* _world, EntityHandle* _entity, const Node3dHandle& _camera);

#endif // ARI_EN_H
