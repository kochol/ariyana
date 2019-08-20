#pragma once
#include "Serialize.hpp"
#include "core/memory/Memory.hpp"

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

		core::StringAtom function_name;
		RpcType rpc_type;

		/* TODO: Add this function to client and server system.
		 *template <typename P1>
		void Call(P1 p1)
		{
			SetParam1((void*)& p1);
		}*/

    };

	template<typename P1>
	class RPC1: public RPC
	{
	public:

		RPC1()
		{
			p1 = core::Memory::Alloc(sizeof(P1));
		}

		~RPC1() override
		{
			core::Memory::Free(p1);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			return Serialize<P1, Stream>(stream, *p1);
		}

		bool Serialize(void* stream) override
		{
			SerializeRpc(*((yojimbo::WriteStream*)stream));
		}

		bool Deserialize(void* stream) override
		{
			SerializeRpc(*((yojimbo::ReadStream*)stream));
		}

		bool SerializeMeasure(void* stream) override
		{
			SerializeRpc(*((yojimbo::MeasureStream*)stream));
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
