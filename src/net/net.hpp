#pragma once
#include "yojimbo.h"
#include "RPC.hpp"
#include "sx/hash.h"

namespace ari::net
{
	bool InitNetwork();

	void ShutdownNetwork();

  	// a simple test message
	enum class GameMessageType {
		CREATE_ENTITY,
		UPDATE_ENTITY,
		DESTROY_ENTITY,
		RPC_CALL,
		COUNT
	};

	// two channels, one for each type that Yojimbo supports
	enum class GameChannel {
		RELIABLE,
		UNRELIABLE,
		COUNT
	};

	// the client and server config
	struct GameConnectionConfig : yojimbo::ClientServerConfig {
		GameConnectionConfig() {
			numChannels = 2;
			channel[(int)GameChannel::RELIABLE].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
			channel[(int)GameChannel::UNRELIABLE].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
		}
	};

	/// For internal use only.
	void _AddRPC(RPC* rpc);

	/// Returns the RPC pointer by its function hash
	RPC* GetRPC(uint32_t rpc_function_hash);

	// RPC functions
	template<typename P1>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1))
	{
		RPC1<P1>* rpc = core::Memory::New<RPC1<P1>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}


} // namespace ari::net
