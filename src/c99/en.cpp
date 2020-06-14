#include "en.h"
#include "core/memory/Memory.hpp"
#include "en/World.hpp"
#include "en/Node.hpp"
#include "core/log.h"
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
    union { ari::en::EntityHandle cpp; EntityHandle c; } h = { ari::en::World::CreateEntity() };
    log_debug("Entiy h: %d i: %d obj: %p", h.c.Handle, h.c.Index, h.c.Entity);
    return h.c;
}

void DeleteEntityWorld(EntityHandle* _entity)
{
    log_debug("Entiy h: %d i: %d obj: %p", _entity->Handle, _entity->Index, _entity->Entity);
    const union { EntityHandle c; ari::en::EntityHandle cpp; } h = { *_entity };
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

void NodeAddChild(void* _node, void* _child)
{
    reinterpret_cast<ari::en::Node*>(_node)->AddChild(
        reinterpret_cast<ari::en::Node*>(_child));
}

void NodeRemoveChild(void* _node, void* _child)
{
	reinterpret_cast<ari::en::Node*>(_node)->RemoveChild(
		reinterpret_cast<ari::en::Node*>(_child));
}
