#include "NetworkSystem.hpp"

namespace ari::net
{
	void NetworkSystem::CallRPC_internal(RPC* rpc, int client_index)
	{
		if (client_index == -1)
		{
			a_assert(rpc->rpc_type != RpcType::Client);
			a_assert((rpc->rpc_type == RpcType::Server
				&& m_network_type == SystemNetworkType::Client)
				|| rpc->rpc_type == RpcType::MultiCast);
		}
		else
			a_assert(rpc->rpc_type == RpcType::Client);

		if (rpc->rpc_type == RpcType::MultiCast)
		{
			rpc->Call(); // Also call on sender
			if (m_network_type == SystemNetworkType::Client)
				return;
		}

		SendRPC(rpc, client_index);

	}

} // namespace ari::net
