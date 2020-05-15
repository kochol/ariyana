#ifndef ARI_NET_H
#define ARI_NET_H

#include "macros.h"

struct EntityHandle;
struct Node3dHandle;

// Globals
CARI_API bool InitNetwork();
CARI_API void ShutdownNetwork();

// Server System
CARI_API void* CreateServerSystem();
CARI_API void DeleteServerSystem(void* _obj);
CARI_API bool CreateServerServerSystem(void* _obj, char* ip, int port);
CARI_API void StopServerSystem(void* _obj);

// Client System
CARI_API void* CreateClientSystem();
CARI_API void DeleteClientSystem(void* _obj);
CARI_API void ConnectClientSystem(void* _obj, char* ip, int port);
CARI_API void StopClientSystem(void* _obj);

// PropertyReplicator
CARI_COMPONENT_HANDLE(PropertyReplicatorHandle)
CARI_API PropertyReplicatorHandle CreatePropertyReplicatorComponent();
//CARI_API void DeletePropertyReplicator(void* _obj);
CARI_API bool IsValidPropertyReplicator(uint32_t& _handle);
CARI_API void AddPropertyReplicatorToWorld(void* _world, EntityHandle* _entity, PropertyReplicatorHandle* _cmp);
CARI_API void AddNode3dProperty(void* _obj, Node3dHandle* _node, char* _propertyName);

#endif // ARI_NET_H
