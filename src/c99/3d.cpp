#include "3d.h"
#include "3d/Camera.hpp"
#include "3d/BoxShape.hpp"
#include "3d/RenderSystem.hpp"
#include "3d/SceneSystem.hpp"

// System functions
void* CreateRenderSystem()
{
    return ari::core::Memory::New<ari::en::RenderSystem>();
}

void* CreateSceneSystem()
{
    return ari::core::Memory::New<ari::en::SceneSystem>();
}

// Node3D
bool IsValidNode3D(uint32_t& _handle)
{
    return ari::core::HandleManager<ari::en::Node3D>::IsHandleValid(_handle);
}

Node3dMembers GetNode3dMembers(void* _node)
{
    auto node = reinterpret_cast<ari::en::Node3D*>(_node);
    return { reinterpret_cast<Vector3*>(&node->Position),
        reinterpret_cast<Quat*>(&node->Rotation),
        reinterpret_cast<Vector3*>(&node->Scale) };
}

// Camera
Node3dHandle CreateCameraComponent()
{
	const auto c = ari::en::World::CreateComponent<ari::en::Camera, ari::en::Node3D>();
    return { c.Handle, c.Index, c.Component };
}

void AddCameraToWorld(void* _world, EntityHandle* _entity, const Node3dHandle& _camera)
{
    const union { Node3dHandle c{}; ari::en::ComponentHandle<ari::en::Camera> cpp; } cam = { _camera };
    const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
    reinterpret_cast<ari::en::World*>(_world)->AddDerivedComponent<ari::en::Camera, ari::en::Node3D>(en.cpp, cam.cpp);
}

CameraMembers GetCameraMembers(void* _node)
{
    auto node = reinterpret_cast<ari::en::Camera*>(_node);
    return {
        reinterpret_cast<Vector3*>(&node->Target),
        reinterpret_cast<Vector3*>(&node->Up),
        reinterpret_cast<Vector3*>(&node->Right),
        &node->AspectRatio,
        &node->Fov,
        &node->xMag,
        &node->yMag,
        &node->zNear,
        &node->zFar,
    };
}

// BoxShape
Node3dHandle CreateBoxShapeComponent()
{
    const auto c = ari::en::World::CreateComponent<ari::en::BoxShape, ari::en::Node3D>();
    return { c.Handle, c.Index, c.Component };
}

void AddBoxShapeToWorld(void* _world, EntityHandle* _entity, const Node3dHandle& _box)
{
    const union { Node3dHandle c{}; ari::en::ComponentHandle<ari::en::BoxShape> cpp; } box = { _box };
    const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
    reinterpret_cast<ari::en::World*>(_world)->AddDerivedComponent<ari::en::BoxShape, ari::en::Node3D>(en.cpp, box.cpp);
}

BoxShapeMembers GetBoxShapeMembers(void* _node)
{
    auto node = reinterpret_cast<ari::en::BoxShape*>(_node);
    return {
        reinterpret_cast<TextureHandle*>(&node->Texture),
        reinterpret_cast<SubMeshHandle*>(&node->SubMesh)
    };
}