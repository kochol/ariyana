#include "net.h"
#include "en.h"
#include "3d.h"
#include "net/ServerSystem.hpp"
#include "net/ClientSystem.hpp"
#include "3d/BoxShape.hpp"
#include "net/Serialize.hpp"
#include "yojimbo.h"
#include "netSerializer.hpp"

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

// bool
bool SerializeBool(void* _stream, bool* _val)
{
    return SerializeBool(*((yojimbo::WriteStream*)_stream), *_val);
}
bool SerializeMeasureBool(void* _stream, bool* _val)
{
    return SerializeBool(*((yojimbo::MeasureStream*)_stream), *_val);
}
bool DeserializeBool(void* _stream, bool* _val)
{
    return SerializeBool(*((yojimbo::ReadStream*)_stream), *_val);
}

// int32
bool SerializeInt32(void* _stream, int* _val)
{
    return SerializeBits(*((yojimbo::WriteStream*)_stream), *(uint32_t*)_val, 32);
}
bool SerializeMeasureInt32(void* _stream, int* _val)
{
    return SerializeBits(*((yojimbo::MeasureStream*)_stream), *(uint32_t*)_val, 32);
}
bool DeserializeInt32(void* _stream, int* _val)
{
    return SerializeBits(*((yojimbo::ReadStream*)_stream), *(uint32_t*)_val, 32);
}

// uint32
bool SerializeUint32(void* _stream, uint32_t* _val)
{
    return SerializeBits(*((yojimbo::WriteStream*)_stream), *_val, 32);
}
bool SerializeMeasureUint32(void* _stream, uint32_t* _val)
{
    return SerializeBits(*((yojimbo::MeasureStream*)_stream), *_val, 32);
}
bool DeserializeUint32(void* _stream, uint32_t* _val)
{
    return SerializeBits(*((yojimbo::ReadStream*)_stream), *_val, 32);
}

// uint64
bool SerializeUint64(void* _stream, uint64_t* _val)
{
    return SerializeUInt64(*((yojimbo::WriteStream*)_stream), *_val);
}
bool SerializeMeasureUint64(void* _stream, uint64_t* _val)
{
    return SerializeUInt64(*((yojimbo::MeasureStream*)_stream), *_val);
}
bool DeserializeUint64(void* _stream, uint64_t* _val)
{
    return SerializeUInt64(*((yojimbo::ReadStream*)_stream), *_val);
}

// float
bool SerializeFloat(void* _stream, float* _val)
{
    return SerializeFloat(*((yojimbo::WriteStream*)_stream), *_val);
}
bool SerializeMeasureFloat(void* _stream, float* _val)
{
    return SerializeFloat(*((yojimbo::MeasureStream*)_stream), *_val);
}
bool DeserializeFloat(void* _stream, float* _val)
{
    return SerializeFloat(*((yojimbo::ReadStream*)_stream), *_val);
}

// double
bool SerializeDouble(void* _stream, double* _val)
{
    return SerializeDouble(*((yojimbo::WriteStream*)_stream), *_val);
}
bool SerializeMeasureDouble(void* _stream, double* _val)
{
    return SerializeDouble(*((yojimbo::MeasureStream*)_stream), *_val);
}
bool DeserializeDouble(void* _stream, double* _val)
{
    return SerializeDouble(*((yojimbo::ReadStream*)_stream), *_val);
}

// string
bool SerializeString(void* _stream, char* _val, int _size)
{
    return SerializeString(*((yojimbo::WriteStream*)_stream), _val, _size);
}
bool SerializeMeasureString(void* _stream, char* _val, int _size)
{
    return SerializeString(*((yojimbo::MeasureStream*)_stream), _val, _size);
}
bool DeserializeString(void* _stream, char* _val, int _size)
{
    return SerializeString(*((yojimbo::ReadStream*)_stream), _val, _size);
}

// bits
bool SerializeBits(void* _stream, void* _val, int _size)
{
    return SerializeBits(*((yojimbo::WriteStream*)_stream), *(uint32_t*)_val, _size);
}
bool SerializeMeasureBits(void* _stream, void* _val, int _size)
{
    return SerializeBits(*((yojimbo::MeasureStream*)_stream), *(uint32_t*)_val, _size);
}
bool DeserializeBits(void* _stream, void* _val, int _size)
{
    return SerializeBits(*((yojimbo::ReadStream*)_stream), *(uint32_t*)_val, _size);
}

// bytes
bool SerializeBytes(void* _stream, void* _val, int _size)
{
    return SerializeBytes(*((yojimbo::WriteStream*)_stream), _val, _size);
}
bool SerializeMeasureBytes(void* _stream, void* _val, int _size)
{
    return SerializeBytes(*((yojimbo::MeasureStream*)_stream), _val, _size);
}
bool DeserializeBytes(void* _stream, void* _val, int _size)
{
    return SerializeBytes(*((yojimbo::ReadStream*)_stream), _val, _size);
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
