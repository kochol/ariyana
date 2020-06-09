#ifndef ARI_2D_H
#define ARI_2D_H

#include "en.h"

// System functions
CARI_API void* CreateRenderSystem2D();
CARI_API void* CreateSceneSystem2D();

// Node2D
CARI_COMPONENT_HANDLE(Node2dHandle)
struct Node2dMembers
{
	Vector2* Position;
	float* Rotation;
	Vector2* Scale;
};
CARI_API bool IsValidNode2D(uint32_t& _handle);
CARI_API Node2dMembers GetNode2dMembers(void* _node);

// Camera2D
CARI_API Node2dHandle CreateCamera2dComponent();
CARI_API void AddCamera2dToWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _camera);
CARI_API void RemoveCamera2dFromWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _camera, bool _dispose);

// Sprite2D
struct Sprite2dMembers
{
	TextureHandle	*	Texture;
	Color			*	v4Color;
	Quat			*	UV;
};
CARI_API Node2dHandle CreateSprite2dComponent();
CARI_API void AddSprite2dToWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _node);
CARI_API void RemoveSprite2dFromWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _node, bool _dispose);
CARI_API Sprite2dMembers GetSprite2dMembers(void* _node);

// Canvas
struct CanvasMembers
{
	RectI* Rect;
};
CARI_API Node2dHandle CreateCanvasComponent();
CARI_API void AddCanvasToWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _node);
CARI_API void RemoveCanvasFromWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _node, bool _dispose);
CARI_API CanvasMembers GetCanvasMembers(void* _node);

#endif // ARI_2D_H