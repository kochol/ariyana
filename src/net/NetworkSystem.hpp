#pragma once
#include "yojimbo.h"
#include "core/string/StringAtom.hpp"
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
		core::Map<core::StringAtom, RPC*> m_mRpcs;

		SystemNetworkType m_network_type;

		virtual void SendRPC(RPC* rpc) = 0;

		void CallRPC_internal(RPC* rpc);

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
			rpc->SetParam1((void*)& p1);
			CallRPC_internal(rpc);
		}
    };

} // namespace ari::net
