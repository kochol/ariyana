#include "net.h"
#include "net/ServerSystem.hpp"
#include "net/ClientSystem.hpp"

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
