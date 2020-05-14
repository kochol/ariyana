#ifndef ARI_NET_H
#define ARI_NET_H

#include "macros.h"

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

#endif // ARI_NET_H
