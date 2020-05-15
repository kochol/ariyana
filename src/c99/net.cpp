#include "net.h"
#include "en.h"
#include "net/ServerSystem.hpp"
#include "net/ClientSystem.hpp"
#include "3d/BoxShape.hpp"

// Globals
bool InitNetwork()
{
    return ari::net::InitNetwork();
}

void ShutdownNetwork()
{
    ari::net::ShutdownNetwork();
}

// Server System
void* CreateServerSystem()
{
    return ari::core::Memory::New<ari::net::ServerSystem>();
}

void DeleteServerSystem(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::net::ServerSystem*>(_obj));
}

bool CreateServerServerSystem(void* _obj, char* ip, int port)
{
    return reinterpret_cast<ari::net::ServerSystem*>(_obj)->CreateServer(yojimbo::Address(ip, port));
}

void StopServerSystem(void* _obj)
{
    return reinterpret_cast<ari::net::ServerSystem*>(_obj)->StopServer();
}

// Client System
void* CreateClientSystem()
{
    return ari::core::Memory::New<ari::net::ClientSystem>();
}

void DeleteClientSystem(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::net::ClientSystem*>(_obj));
}

void ConnectClientSystem(void* _obj, char* ip, int port)
{
    reinterpret_cast<ari::net::ClientSystem*>(_obj)->Connect(yojimbo::Address(ip, port));
}

void StopClientSystem(void* _obj)
{
    reinterpret_cast<ari::net::ClientSystem*>(_obj)->StopClient();
}

// PropertyReplicator
PropertyReplicatorHandle CreatePropertyReplicatorComponent()
{
    union 
    {
        ari::en::ComponentHandle<ari::net::PropertyReplicator> cpp;
        PropertyReplicatorHandle c;
    } h = { ari::en::World::CreateComponent<ari::net::PropertyReplicator>() };
    return h.c;
}

/*void DeletePropertyReplicator(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::net::PropertyReplicator*>(_obj));
}*/

bool IsValidPropertyReplicator(uint32_t& _handle)
{
    return ari::core::HandleManager<ari::net::PropertyReplicator>::IsHandleValid(_handle);
}

void AddPropertyReplicatorToWorld(void* _world, EntityHandle* _entity, PropertyReplicatorHandle* _cmp)
{
    const union { PropertyReplicatorHandle c{}; ari::en::ComponentHandle<ari::net::PropertyReplicator> cpp; } cmp = { *_cmp };
    const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
    reinterpret_cast<ari::en::World*>(_world)->AddComponent<ari::net::PropertyReplicator>(en.cpp, cmp.cpp);
}

void AddNode3dProperty(void* _obj, Node3dHandle* _node, char* _propertyName)
{
    const union { Node3dHandle c{}; ari::en::ComponentHandle<ari::en::Node3D> cpp; } node = { *_node };
    reinterpret_cast<ari::net::PropertyReplicator*>(_obj)->AddProperty(node.cpp, _propertyName);
}
