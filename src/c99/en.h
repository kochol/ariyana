#ifndef ARI_EN_H
#define ARI_EN_H

#include "macros.h"
#include "core.h"
#include "gfx.h"

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
CARI_API bool* GetEntityMembers(EntityHandle* _entity);

// Node
CARI_API const char* GetNodeName(void* _node);
CARI_API void SetNodeName(void* _node, char* _name);

// Node3D
CARI_COMPONENT_HANDLE(Node3dHandle)
struct Node3dMembers
{
	Vector3* Position;
	Quat* Rotation;
	Vector3* Scale;
};
CARI_API bool IsValidNode3D(uint32_t& _cameraHandle);
CARI_API Node3dMembers GetNode3dMembers(void* _node);

// Camera
struct CameraMembers
{
	Vector3	*	Target,
			*	Up,
			*	Right;
	float	*	AspectRatio;
	float	*	Fov;
	float	*	xMag;
	float	*	yMag;
	float	*	zNear;
	float	*	zFar;
	//CameraType*	Type;
};
CARI_API Node3dHandle CreateCameraComponent();
CARI_API void AddCameraToWorld(void* _world, EntityHandle* _entity, const Node3dHandle& _camera);
CARI_API CameraMembers GetCameraMembers(void* _node);

// BoxShape
struct BoxShapeMembers
{
	TextureHandle* Texture;
	SubMeshHandle* SubMesh;
};
CARI_API Node3dHandle CreateBoxShapeComponent();
CARI_API void AddBoxShapeToWorld(void* _world, EntityHandle* _entity, const Node3dHandle& _camera);
CARI_API BoxShapeMembers GetBoxShapeMembers(void* _node);

#endif // ARI_EN_H
