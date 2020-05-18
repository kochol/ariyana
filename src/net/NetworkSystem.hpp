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

		virtual void SendRPC(RPC* rpc, int client_index = -1) = 0;

		void CallRPC_internal(RPC* rpc, int client_index = -1);

    public:

		virtual void Call_C_RPC(void* rpc, bool _reliable, RpcType _rpc_type, int client_id) = 0;

		template<typename P1>
		void CallRPC(RPC* rpc, P1 p1)
		{
			rpc->SetParam1((void*)& p1);
			CallRPC_internal(rpc);
		}

		template<typename P1>
		void CallRPC(int client_index, RPC* rpc, P1 p1)
		{
			rpc->SetParam1((void*)& p1);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2>
		void CallRPC(RPC* rpc, P1 p1, P2 p2)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2, typename P3>
		void CallRPC(RPC* rpc, P1 p1, P2 p2, P3 p3)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2, typename P3>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2, P3 p3)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2, typename P3, typename P4>
		void CallRPC(RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2, typename P3, typename P4>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		void CallRPC(RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		void CallRPC(RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		void CallRPC(RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			rpc->SetParam7((void*)& p7);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			rpc->SetParam7((void*)& p7);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
		void CallRPC(RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			rpc->SetParam7((void*)& p7);
			rpc->SetParam8((void*)& p8);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			rpc->SetParam7((void*)& p7);
			rpc->SetParam8((void*)& p8);
			CallRPC_internal(rpc, client_index);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
		void CallRPC(RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			rpc->SetParam7((void*)& p7);
			rpc->SetParam8((void*)& p8);
			rpc->SetParam9((void*)& p9);
			CallRPC_internal(rpc);
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
		void CallRPC(int client_index, RPC* rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
		{
			rpc->SetParam1((void*)& p1);
			rpc->SetParam2((void*)& p2);
			rpc->SetParam3((void*)& p3);
			rpc->SetParam4((void*)& p4);
			rpc->SetParam5((void*)& p5);
			rpc->SetParam6((void*)& p6);
			rpc->SetParam7((void*)& p7);
			rpc->SetParam8((void*)& p8);
			rpc->SetParam9((void*)& p9);
			CallRPC_internal(rpc, client_index);
		}

    };

} // namespace ari::net
