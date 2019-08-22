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

	/// Returns the client index of called RPC function.
	int GetLastRpcClientIndex();

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


	template <typename P1, typename P2>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2))
	{
		RPC2<P1, P2>* rpc = core::Memory::New<RPC2<P1, P2>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

	template <typename P1, typename P2, typename P3>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2, P3))
	{
		RPC3<P1, P2, P3>* rpc = core::Memory::New<RPC3<P1, P2, P3>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

	template <typename P1, typename P2, typename P3, typename P4>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2, P3, P4))
	{
		RPC4<P1, P2, P3, P4>* rpc = core::Memory::New<RPC4<P1, P2, P3, P4>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2, P3, P4, P5))
	{
		RPC5<P1, P2, P3, P4, P5>* rpc = core::Memory::New<RPC5<P1, P2, P3, P4, P5>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2, P3, P4, P5, P6))
	{
		RPC6<P1, P2, P3, P4, P5, P6>* rpc = core::Memory::New<RPC6<P1, P2, P3, P4, P5, P6>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2, P3, P4, P5, P6, P7))
	{
		RPC7<P1, P2, P3, P4, P5, P6, P7>* rpc = core::Memory::New<RPC7<P1, P2, P3, P4, P5, P6, P7>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2, P3, P4, P5, P6, P7, P8))
	{
		RPC8<P1, P2, P3, P4, P5, P6, P7, P8>* rpc = core::Memory::New<RPC8<P1, P2, P3, P4, P5, P6, P7, P8>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	RPC* AddRPC(const core::String& function_name, RpcType rpc_type,
		void(*_fn)(P1, P2, P3, P4, P5, P6, P7, P8, P9))
	{
		RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9>* rpc = core::Memory::New<RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9>>();
		rpc->Fn = _fn;
		rpc->function_name = function_name;
		rpc->rpc_type = rpc_type;
		rpc->function_hash = sx_hash_fnv32_str(function_name.AsCStr());
		_AddRPC(rpc);
		return rpc;
	}

} // namespace ari::net
