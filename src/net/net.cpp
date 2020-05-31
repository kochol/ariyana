#include "net.hpp"
#include "core/containers/Map.hpp"

namespace ari::net
{
	core::Map<uint32_t, RPC*> g_mRpcs;
	int g_iLastRpcClientIndex = -1;

	serialize_cb* g_on_serialize = nullptr;
	serialize_cb* g_on_serialize_measure = nullptr;
	deserialize_cb* g_on_deserialize = nullptr;
	call_c_rpc_cb* g_on_call_rpc = nullptr;
	call_c_rpc_cb* g_on_delete_rpc = nullptr;
	call_c_rpc_cb* g_on_add_ref_rpc = nullptr;

	//------------------------------------------------------------------------------
	bool InitNetwork()
	{
		return InitializeYojimbo();
	}

	//------------------------------------------------------------------------------
	void ShutdownNetwork()
	{
		for (auto rpc : g_mRpcs)
			core::Memory::Delete(rpc.value);
		g_mRpcs.Clear();
		ShutdownYojimbo();
	}

	//------------------------------------------------------------------------------
	void _AddRPC(RPC* rpc)
	{
		g_mRpcs.Add(rpc->function_hash, rpc);
	}

	//------------------------------------------------------------------------------
	RPC* GetRPC(uint32_t rpc_function_hash)
	{
		return g_mRpcs[rpc_function_hash];
	}

	int GetLastRpcClientIndex()
	{
		return g_iLastRpcClientIndex;
	}

}
