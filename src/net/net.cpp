#include "net.hpp"
#include "core/containers/Map.hpp"

namespace ari::net
{
	core::Map<uint32_t, RPC*> g_mRpcs;

	//------------------------------------------------------------------------------
	bool InitNetwork()
	{
		return InitializeYojimbo();
	}

	//------------------------------------------------------------------------------
	void ShutdownNetwork()
	{
		ShutdownYojimbo();
	}

	void _AddRPC(RPC* rpc)
	{
		g_mRpcs.Add(rpc->function_hash, rpc);
	}

	RPC* GetRPC(uint32_t rpc_function_hash)
	{
		return g_mRpcs[rpc_function_hash];
	}

}
