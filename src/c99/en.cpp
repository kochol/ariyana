#include "en.h"
#include "core/memory/Memory.hpp"
#include "en/World.hpp"
#include "3d/RenderSystem.hpp"
#include "3d/SceneSystem.hpp"

void* CreateWorld()
{    
    return ari::core::Memory::New<ari::en::World>();
}

void DeleteWorld(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::en::World*>(_obj));
}

void UpdateWorld(void* _obj, float _elapsedTime)
{
    reinterpret_cast<ari::en::World*>(_obj)->Update(_elapsedTime);
}

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
