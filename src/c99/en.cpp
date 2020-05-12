#include "en.h"
#include "core/memory/Memory.hpp"
#include "en/World.hpp"
#include "3d/RenderSystem.hpp"
#include "3d/SceneSystem.hpp"

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
