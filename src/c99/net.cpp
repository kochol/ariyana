#include "net.h"
#include "en.h"
#include "net/ServerSystem.hpp"
#include "net/ClientSystem.hpp"
#include "3d/BoxShape.hpp"
#include "net/Serialize.hpp"

// Globals
bool InitNetworkLink
(
    serialize_cb* _on_serialize,
    deserialize_cb* _on_deserialize,
    serialize_cb* _on_serialize_measure,
    call_rpc_cb* _on_call_rpc
)
{
    ari::net::g_on_serialize = _on_serialize;
    ari::net::g_on_deserialize = _on_deserialize;
    ari::net::g_on_serialize_measure = _on_serialize_measure;
    ari::net::g_on_call_rpc = _on_call_rpc;
    return ari::net::InitNetwork();
}

void ShutdownNetwork()
{
    ari::net::ShutdownNetwork();
}

// Serialize stuffs

// uint32
bool SerializeUint32(void* _stream, uint32_t& _val)
{
    return ari::net::Serialize(*((yojimbo::WriteStream*)_stream), _val);
}
bool SerializeMeasureUint32(void* _stream, uint32_t& _val)
{
    return ari::net::Serialize(*((yojimbo::MeasureStream*)_stream), _val);
}
bool DeserializeUint32(void* _stream, uint32_t& _val)
{
    return ari::net::Serialize(*((yojimbo::ReadStream*)_stream), _val);
}

// RPC stuff
class CRPC: ari::net::RPC
{
    // Inherited via RPC
    virtual bool Serialize(void* stream) override
    {
        return false;
    }

    virtual bool Deserialize(void* stream) override
    {
        return false;
    }

    virtual bool SerializeMeasure(void* stream) override
    {
        return false;
    }

    virtual void Call() override
    {
    }
};

void AddRpc(AriRPC* _rpc)
{
	
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
    reinterpret_cast<ari::net::ServerSystem*>(_obj)->StopServer();
}

void CallCRPCServerSystem(void* _obj, void* _rpc, bool _reliable, RpcType _rpc_type, int client_id)
{
    reinterpret_cast<ari::net::ServerSystem*>(_obj)->Call_C_RPC(_rpc, _reliable, ari::net::RpcType(_rpc_type), client_id);
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

void CallCRPCClientSystem(void* _obj, void* _rpc, bool _reliable, RpcType _rpc_type)
{
    reinterpret_cast<ari::net::ClientSystem*>(_obj)->Call_C_RPC(_rpc, _reliable, ari::net::RpcType(_rpc_type), 0);
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
