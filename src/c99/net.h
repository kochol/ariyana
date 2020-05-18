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
// uint32
CARI_API bool SerializeUint32(void* _stream, uint32_t& _val);
CARI_API bool SerializeMeasureUint32(void* _stream, uint32_t& _val);
CARI_API bool DeserializeUint32(void* _stream, uint32_t& _val);

// RPC stuffs
enum RpcType
{
	RpcType_Server,
	RpcType_Client,
	RpcType_MultiCast,
};

struct AriRPC
{
	void* rpc;
	char* function_name;
	uint32_t function_hash;
	RpcType rpc_type;
};

CARI_API void AddRpc(AriRPC* _rpc);

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
