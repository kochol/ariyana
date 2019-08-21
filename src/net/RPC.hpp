#pragma once
#include "Serialize.hpp"
#include "core/memory/Memory.hpp"
#include "core/string/String.hpp"

namespace ari::net
{
	enum class RpcType
	{
		Server, // Called by client only runs on server
		Client, // Called by server only runs on particular client
		MultiCast // Called by server and runs on both server and all connected clients
	};

    class RPC
    {
    public:

		virtual ~RPC() = default;

		virtual bool Serialize(void* stream) = 0;
		virtual bool Deserialize(void* stream) = 0;
		virtual bool SerializeMeasure(void* stream) = 0;

		// Call the function
		virtual void Call() = 0;

		virtual void SetParam1(void* param1) {}

		core::String function_name;
		uint32_t function_hash = 0;
		RpcType rpc_type;
		bool Reliable = false;
    };

	template<typename P1>
	class RPC1: public RPC
	{
	public:

		RPC1()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
		}

		~RPC1() override
		{
			core::Memory::Free(p1);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			return ari::net::Serialize<P1, Stream>(stream, *p1);
		}

		bool Serialize(void* stream) override
		{
			return SerializeRpc(*((yojimbo::WriteStream*)stream));
		}

		bool Deserialize(void* stream) override
		{
			return SerializeRpc(*((yojimbo::ReadStream*)stream));
		}

		bool SerializeMeasure(void* stream) override
		{
			return SerializeRpc(*((yojimbo::MeasureStream*)stream));
		}

		void Call() override
		{
			Fn(*p1);
		}

		void SetParam1(void* param1) override
		{
			*p1 = *(P1*)param1;
		}
		
		void(*Fn)(P1);
		P1* p1 = nullptr;
	};

} // namespace ari::net
