#ifndef ARI_NET_H
#define ARI_NET_H

#include "macros.h"

struct EntityHandle;
struct Node3dHandle;

// Globals
typedef bool (serialize_cb)(void* _stream, void* _rpc);
typedef bool (deserialize_cb)(void* _stream, int* _out_index);
typedef void (call_rpc_cb)(int rpc_index);
CARI_API bool InitNetworkLink
	(
		serialize_cb* _on_serialize, 
		deserialize_cb* _on_deserialize, 
		serialize_cb* _on_serialize_measure,
		call_rpc_cb* _on_call_rpc
	);
CARI_API void ShutdownNetwork();

// Serialize stuffs
// bool
CARI_API bool SerializeBool(void* _stream, bool* _val);
CARI_API bool SerializeMeasureBool(void* _stream, bool* _val);
CARI_API bool DeserializeBool(void* _stream, bool* _val);
// int32
CARI_API bool SerializeInt32(void* _stream, int* _val);
CARI_API bool SerializeMeasureInt32(void* _stream, int* _val);
CARI_API bool DeserializeInt32(void* _stream, int* _val);
// uint32
CARI_API bool SerializeUint32(void* _stream, uint32_t* _val);
CARI_API bool SerializeMeasureUint32(void* _stream, uint32_t* _val);
CARI_API bool DeserializeUint32(void* _stream, uint32_t* _val);
// uint64
CARI_API bool SerializeUint64(void* _stream, uint64_t* _val);
CARI_API bool SerializeMeasureUint64(void* _stream, uint64_t* _val);
CARI_API bool DeserializeUint64(void* _stream, uint64_t* _val);
// float
CARI_API bool SerializeFloat(void* _stream, float* _val);
CARI_API bool SerializeMeasureFloat(void* _stream, float* _val);
CARI_API bool DeserializeFloat(void* _stream, float* _val);
// double
CARI_API bool SerializeDouble(void* _stream, double* _val);
CARI_API bool SerializeMeasureDouble(void* _stream, double* _val);
CARI_API bool DeserializeDouble(void* _stream, double* _val);
// string
CARI_API bool SerializeString(void* _stream, char* _val, int _size);
CARI_API bool SerializeMeasureString(void* _stream, char* _val, int _size);
CARI_API bool DeserializeString(void* _stream, char* _val, int _size);
// bits
CARI_API bool SerializeBits(void* _stream, void* _val, int _size);
CARI_API bool SerializeMeasureBits(void* _stream, void* _val, int _size);
CARI_API bool DeserializeBits(void* _stream, void* _val, int _size);
// bytes
CARI_API bool SerializeBytes(void* _stream, void* _val, int _size);
CARI_API bool SerializeMeasureBytes(void* _stream, void* _val, int _size);
CARI_API bool DeserializeBytes(void* _stream, void* _val, int _size);

// RPC stuffs
enum RpcType
{
	RpcType_Server,
	RpcType_Client,
	RpcType_MultiCast,
};

// Server System
CARI_API void* CreateServerSystem();
CARI_API void DeleteServerSystem(void* _obj);
CARI_API bool CreateServerServerSystem(void* _obj, char* ip, int port);
CARI_API void StopServerSystem(void* _obj);
CARI_API void CallCRPCServerSystem(void* _obj, void* _rpc, bool _reliable, RpcType _rpc_type, int client_id);

// Client System
CARI_API void* CreateClientSystem();
CARI_API void DeleteClientSystem(void* _obj);
CARI_API void ConnectClientSystem(void* _obj, char* ip, int port);
CARI_API void StopClientSystem(void* _obj);
CARI_API void CallCRPCClientSystem(void* _obj, void* _rpc, bool _reliable, RpcType _rpc_type);

// PropertyReplicator
CARI_COMPONENT_HANDLE(PropertyReplicatorHandle)
CARI_API PropertyReplicatorHandle CreatePropertyReplicatorComponent();
//CARI_API void DeletePropertyReplicator(void* _obj);
CARI_API bool IsValidPropertyReplicator(uint32_t& _handle);
CARI_API void AddPropertyReplicatorToWorld(void* _world, EntityHandle* _entity, PropertyReplicatorHandle* _cmp);
CARI_API void AddNode3dProperty(void* _obj, Node3dHandle* _node, char* _propertyName);

#endif // ARI_NET_H