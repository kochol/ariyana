#include "2d.h"
#include "2d/RenderSystem2D.hpp"
#include "2d/SceneSystem2D.hpp"
#include "2d/Camera2D.hpp"
#include "2d/Sprite.hpp"

// System functions
void* CreateRenderSystem2D()
{
    return ari::core::Memory::New<ari::en::RenderSystem2D>();
}

void* CreateSceneSystem2D()
{
    return ari::core::Memory::New<ari::en::SceneSystem2D>();
}

// Node2D
bool IsValidNode2D(uint32_t& _handle)
{
    return ari::core::HandleManager<ari::en::Node2D>::IsHandleValid(_handle);
}

Node2dMembers GetNode2dMembers(void* _node)
{
    auto node = reinterpret_cast<ari::en::Node2D*>(_node);
    return { reinterpret_cast<Vector2*>(&node->Position),
        &node->Rotation,
        reinterpret_cast<Vector2*>(&node->Scale) };   
}

// Camera2D
Node2dHandle CreateCamera2dComponent()
{
    const union { ari::en::ComponentHandle<ari::en::Camera2D> cpp; Node2dHandle c; } h =
    { ari::en::World::CreateComponent<ari::en::Camera2D, ari::en::Node2D>() };
    return h.c;
}

void AddCamera2dToWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _camera)
{
    union { Node2dHandle c{}; ari::en::ComponentHandle<ari::en::Camera2D> cpp; } cam = { _camera };
    const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
    reinterpret_cast<ari::en::World*>(_world)->AddDerivedComponent<ari::en::Camera2D, ari::en::Node2D>(en.cpp, cam.cpp);
}

void RemoveCamera2dFromWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _camera, bool _dispose)
{
    union { Node2dHandle c{}; ari::en::ComponentHandle<ari::en::Camera2D> cpp; } cam = { _camera };
    const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
    reinterpret_cast<ari::en::World*>(_world)->RemoveComponent(en.cpp, cam.cpp, _dispose);
}

// Sprite2D
Node2dHandle CreateSprite2dComponent()
{
    const union { ari::en::ComponentHandle<ari::en::Sprite> cpp; Node2dHandle c; } h =
    { ari::en::World::CreateComponent<ari::en::Sprite, ari::en::Node2D>() };
    return h.c;
}

void AddSprite2dToWorld(void* _world, EntityHandle* _entity, const Node2dHandle& _node)
{    
    union { Node2dHandle c{}; ari::en::ComponentHandle<ari::en::Sprite> cpp; } node = { _node };
    const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
    reinterpret_cast<ari::en::World*>(_world)->AddDerivedComponent<ari::en::Sprite, ari::en::Node2D>(en.cpp, node.cpp);
}

Sprite2dMembers GetSprite2dMembers(void* _node)
{
    auto node = reinterpret_cast<ari::en::Sprite*>(_node);
    return {
        reinterpret_cast<TextureHandle*>(&node->Texture)
    };
}
