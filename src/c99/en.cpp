#include "en.h"
#include "core/memory/Memory.hpp"
#include "en/World.hpp"
#include "3d/RenderSystem.hpp"
#include "3d/SceneSystem.hpp"
#include "3d/Camera.hpp"
#include "3d/BoxShape.hpp"

// World
void* CreateWorld()
{    
    ari::en::World* w = ari::core::Memory::New<ari::en::World>();
    w->UpdateType = ari::en::World::UpdateType::Sync;
    return w;
}

void DeleteWorld(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::en::World*>(_obj));
}

void UpdateWorld(void* _obj, float _elapsedTime)
{
    reinterpret_cast<ari::en::World*>(_obj)->Update(_elapsedTime);
}

void AddSystemToWorld(void* _world, void* _system)
{
    reinterpret_cast<ari::en::World*>(_world)->AddSystem(
        reinterpret_cast<ari::en::System*>(_system)
        );
}

// System
void* CreateRenderSystem()
{
    return ari::core::Memory::New<ari::en::RenderSystem>();
}

void DeleteSystem(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::en::System*>(_obj));
}

void* CreateSceneSystem()
{
    return ari::core::Memory::New<ari::en::SceneSystem>();
}

// Entity
EntityHandle CreateEntityWorld()
{
	const auto e = ari::en::World::CreateEntity();
    return { e.Handle, e.Index };
}

bool IsValidEntity(uint32_t& _entityHandle)
{
    return ari::core::HandleManager<ari::en::EntityHandle>::IsHandleValid(_entityHandle);
}

// Node3D
bool IsValidNode3D(uint32_t& _cameraHandle)
{
    return ari::core::HandleManager<ari::en::Node3D>::IsHandleValid(_cameraHandle);
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

Vector3* GetCameraTarget(void* _node)
{
    return reinterpret_cast<Vector3*>(&reinterpret_cast<ari::en::Camera*>(_node)->Target);
}

// BoxShape
Node3dHandle CreateBoxShapeComponent()
{
    const auto c = ari::en::World::CreateComponent<ari::en::BoxShape, ari::en::Node3D>();
    return { c.Handle, c.Index, c.Component };
}

void AddBoxShapeToWorld(void* _world, EntityHandle* _entity, const Node3dHandle& _camera)
{
    const union { Node3dHandle c{}; ari::en::ComponentHandle<ari::en::BoxShape> cpp; } cam = { _camera };
    const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
    reinterpret_cast<ari::en::World*>(_world)->AddDerivedComponent<ari::en::BoxShape, ari::en::Node3D>(en.cpp, cam.cpp);
}
