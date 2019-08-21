#pragma once
#include "yojimbo.h"
#include "RPC.hpp"
#include "core/containers/Map.hpp"

namespace ari::en
{
	struct World;
}

namespace ari::net
{
    class NetworkSystem
    {
    protected:

		enum class SystemNetworkType
		{
			Server,
			Client,
			DedicatedServer
		};

    	yojimbo::Adapter* m_pAdapter = nullptr;
		double					m_time = 0.0;
		en::World* m_pWorld = nullptr;
		yojimbo::ClientServerConfig m_connectionConfig;

		SystemNetworkType m_network_type;

		virtual void SendRPC(RPC* rpc, int client_id = -1) = 0;

		void CallRPC_internal(RPC* rpc);

    public:

		template<typename P1>
		void CallRPC(RPC* rpc, P1 p1)
		{
			rpc->SetParam1((void*)& p1);
			CallRPC_internal(rpc);
		}
    };

} // namespace ari::net
