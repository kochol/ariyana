#include "en.h"
#include "core/memory/Memory.hpp"
#include "en/World.hpp"
#include "en/Node.hpp"
#include "en/System.hpp"

// World
void* CreateWorld()
{    
    ari::en::World* w = ari::core::Memory::New<ari::en::World>();
    //w->UpdateType = ari::en::World::UpdateType::Sync;
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

void AddEntityToWorld(void* _world, EntityHandle* _entity)
{
    reinterpret_cast<ari::en::World*>(_world)->AddEntity(
        *(reinterpret_cast<ari::en::EntityHandle*>(_entity)));
}

// System
void DeleteSystem(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::en::System*>(_obj));
}

// Entity
EntityHandle CreateEntityWorld()
{
	const auto e = ari::en::World::CreateEntity();
    return { e.Handle, e.Index, e.entity };
}

void DeleteEntityWorld(EntityHandle _entity)
{
    const union { EntityHandle c; ari::en::EntityHandle cpp; } h = { _entity };
    ari::en::World::DeleteEntity(h.cpp);
}

bool IsValidEntity(uint32_t& _entityHandle)
{
    return ari::core::HandleManager<ari::en::EntityHandle>::IsHandleValid(_entityHandle);
}

bool* GetEntityMembers(EntityHandle* _entity)
{
    union { EntityHandle c; ari::en::EntityHandle cpp; } h = { *_entity };    
    return &h.cpp->bReplicates;
}

// Node
const char* GetNodeName(void* _node)
{
    return reinterpret_cast<ari::en::Node*>(_node)->Name.AsCStr();
}

void SetNodeName(void* _node, char* _name)
{
    return reinterpret_cast<ari::en::Node*>(_node)->Name = _name;
}
