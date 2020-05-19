#ifndef ARI_3D_H
#define ARI_3D_H

#include "en.h"

// System functions
CARI_API void* CreateRenderSystem();
CARI_API void* CreateSceneSystem();

// Node3D
CARI_COMPONENT_HANDLE(Node3dHandle)
struct Node3dMembers
{
	Vector3* Position;
	Quat* Rotation;
	Vector3* Scale;
};
CARI_API bool IsValidNode3D(uint32_t& _handle);
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

#endif // ARI_3D_H
