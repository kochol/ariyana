#pragma once
#include "yojimbo.h"
#include "core/string/StringAtom.hpp"
#include "RPC.hpp"

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
		core::Map<core::StringAtom, RPC*> m_mRpcs;

		SystemNetworkType m_network_type;

		virtual void SendRPC(RPC* rpc) = 0;

    public:

		template<typename P1>
		RPC* AddRPC(const core::StringAtom& function_name, RpcType rpc_type,
			void(*_fn)(P1))
		{
			RPC1<P1>* rpc = core::Memory::New<RPC1<P1>>();
			rpc->Fn = _fn;
			rpc->function_name = function_name;
			rpc->rpc_type = rpc_type;
			m_mRpcs.Add(function_name, rpc);
			return rpc;
		}

		template<typename P1>
		void CallRPC(RPC* rpc, P1 p1)
		{
			a_assert(rpc->rpc_type != RpcType::Client);
			a_assert((rpc->rpc_type == RpcType::Server 
				&& m_network_type == SystemNetworkType::Client)
				|| rpc->rpc_type == RpcType::MultiCast);

			rpc->SetParam1((void*)& p1);
			if (rpc->rpc_type == RpcType::MultiCast)
			{
				rpc->Call(); // Also call on sender
				if (m_network_type == SystemNetworkType::Client)
					return;
			}

			SendRPC(rpc);
		}
    };

} // namespace ari::net
