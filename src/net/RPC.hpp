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

		virtual void SetParam1(void* param) {}
		virtual void SetParam2(void* param) {}
		virtual void SetParam3(void* param) {}
		virtual void SetParam4(void* param) {}
		virtual void SetParam5(void* param) {}
		virtual void SetParam6(void* param) {}
		virtual void SetParam7(void* param) {}
		virtual void SetParam8(void* param) {}
		virtual void SetParam9(void* param) {}

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


	template <typename P1, typename P2>
	class RPC2 : public RPC
	{
	public:

		RPC2()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
		}

		~RPC2() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			return true;
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
			Fn(*p1, *p2);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void(*Fn)(P1, P2);

		P1* p1 = nullptr;
		P2* p2 = nullptr;

	};


	template <typename P1, typename P2, typename P3>
	class RPC3 : public RPC
	{
	public:

		RPC3()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
			p3 = (P3*)core::Memory::Alloc(sizeof(P3));
		}

		~RPC3() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
			core::Memory::Free(p3);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			if (!ari::net::Serialize<P3, Stream>(stream, *p3))
				return false;
			return true;
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
			Fn(*p1, *p2, *p3);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void SetParam3(void* param) override
		{
			*p3 = *(P3*)param;
		}

		void(*Fn)(P1, P2, P3);

		P1* p1 = nullptr;
		P2* p2 = nullptr;
		P3* p3 = nullptr;

	};


	template <typename P1, typename P2, typename P3, typename P4>
	class RPC4 : public RPC
	{
	public:

		RPC4()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
			p3 = (P3*)core::Memory::Alloc(sizeof(P3));
			p4 = (P4*)core::Memory::Alloc(sizeof(P4));
		}

		~RPC4() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
			core::Memory::Free(p3);
			core::Memory::Free(p4);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			if (!ari::net::Serialize<P3, Stream>(stream, *p3))
				return false;
			if (!ari::net::Serialize<P4, Stream>(stream, *p4))
				return false;
			return true;
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
			Fn(*p1, *p2, *p3, *p4);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void SetParam3(void* param) override
		{
			*p3 = *(P3*)param;
		}

		void SetParam4(void* param) override
		{
			*p4 = *(P4*)param;
		}

		void(*Fn)(P1, P2, P3, P4);

		P1* p1 = nullptr;
		P2* p2 = nullptr;
		P3* p3 = nullptr;
		P4* p4 = nullptr;

	};


	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	class RPC5 : public RPC
	{
	public:

		RPC5()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
			p3 = (P3*)core::Memory::Alloc(sizeof(P3));
			p4 = (P4*)core::Memory::Alloc(sizeof(P4));
			p5 = (P5*)core::Memory::Alloc(sizeof(P5));
		}

		~RPC5() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
			core::Memory::Free(p3);
			core::Memory::Free(p4);
			core::Memory::Free(p5);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			if (!ari::net::Serialize<P3, Stream>(stream, *p3))
				return false;
			if (!ari::net::Serialize<P4, Stream>(stream, *p4))
				return false;
			if (!ari::net::Serialize<P5, Stream>(stream, *p5))
				return false;
			return true;
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
			Fn(*p1, *p2, *p3, *p4, *p5);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void SetParam3(void* param) override
		{
			*p3 = *(P3*)param;
		}

		void SetParam4(void* param) override
		{
			*p4 = *(P4*)param;
		}

		void SetParam5(void* param) override
		{
			*p5 = *(P5*)param;
		}

		void(*Fn)(P1, P2, P3, P4, P5);

		P1* p1 = nullptr;
		P2* p2 = nullptr;
		P3* p3 = nullptr;
		P4* p4 = nullptr;
		P5* p5 = nullptr;

	};


	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	class RPC6 : public RPC
	{
	public:

		RPC6()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
			p3 = (P3*)core::Memory::Alloc(sizeof(P3));
			p4 = (P4*)core::Memory::Alloc(sizeof(P4));
			p5 = (P5*)core::Memory::Alloc(sizeof(P5));
			p6 = (P6*)core::Memory::Alloc(sizeof(P6));
		}

		~RPC6() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
			core::Memory::Free(p3);
			core::Memory::Free(p4);
			core::Memory::Free(p5);
			core::Memory::Free(p6);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			if (!ari::net::Serialize<P3, Stream>(stream, *p3))
				return false;
			if (!ari::net::Serialize<P4, Stream>(stream, *p4))
				return false;
			if (!ari::net::Serialize<P5, Stream>(stream, *p5))
				return false;
			if (!ari::net::Serialize<P6, Stream>(stream, *p6))
				return false;
			return true;
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
			Fn(*p1, *p2, *p3, *p4, *p5, *p6);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void SetParam3(void* param) override
		{
			*p3 = *(P3*)param;
		}

		void SetParam4(void* param) override
		{
			*p4 = *(P4*)param;
		}

		void SetParam5(void* param) override
		{
			*p5 = *(P5*)param;
		}

		void SetParam6(void* param) override
		{
			*p6 = *(P6*)param;
		}

		void(*Fn)(P1, P2, P3, P4, P5, P6);

		P1* p1 = nullptr;
		P2* p2 = nullptr;
		P3* p3 = nullptr;
		P4* p4 = nullptr;
		P5* p5 = nullptr;
		P6* p6 = nullptr;

	};


	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	class RPC7 : public RPC
	{
	public:

		RPC7()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
			p3 = (P3*)core::Memory::Alloc(sizeof(P3));
			p4 = (P4*)core::Memory::Alloc(sizeof(P4));
			p5 = (P5*)core::Memory::Alloc(sizeof(P5));
			p6 = (P6*)core::Memory::Alloc(sizeof(P6));
			p7 = (P7*)core::Memory::Alloc(sizeof(P7));
		}

		~RPC7() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
			core::Memory::Free(p3);
			core::Memory::Free(p4);
			core::Memory::Free(p5);
			core::Memory::Free(p6);
			core::Memory::Free(p7);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			if (!ari::net::Serialize<P3, Stream>(stream, *p3))
				return false;
			if (!ari::net::Serialize<P4, Stream>(stream, *p4))
				return false;
			if (!ari::net::Serialize<P5, Stream>(stream, *p5))
				return false;
			if (!ari::net::Serialize<P6, Stream>(stream, *p6))
				return false;
			if (!ari::net::Serialize<P7, Stream>(stream, *p7))
				return false;
			return true;
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
			Fn(*p1, *p2, *p3, *p4, *p5, *p6, *p7);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void SetParam3(void* param) override
		{
			*p3 = *(P3*)param;
		}

		void SetParam4(void* param) override
		{
			*p4 = *(P4*)param;
		}

		void SetParam5(void* param) override
		{
			*p5 = *(P5*)param;
		}

		void SetParam6(void* param) override
		{
			*p6 = *(P6*)param;
		}

		void SetParam7(void* param) override
		{
			*p7 = *(P7*)param;
		}

		void(*Fn)(P1, P2, P3, P4, P5, P6, P7);

		P1* p1 = nullptr;
		P2* p2 = nullptr;
		P3* p3 = nullptr;
		P4* p4 = nullptr;
		P5* p5 = nullptr;
		P6* p6 = nullptr;
		P7* p7 = nullptr;

	};


	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	class RPC8 : public RPC
	{
	public:

		RPC8()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
			p3 = (P3*)core::Memory::Alloc(sizeof(P3));
			p4 = (P4*)core::Memory::Alloc(sizeof(P4));
			p5 = (P5*)core::Memory::Alloc(sizeof(P5));
			p6 = (P6*)core::Memory::Alloc(sizeof(P6));
			p7 = (P7*)core::Memory::Alloc(sizeof(P7));
			p8 = (P8*)core::Memory::Alloc(sizeof(P8));
		}

		~RPC8() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
			core::Memory::Free(p3);
			core::Memory::Free(p4);
			core::Memory::Free(p5);
			core::Memory::Free(p6);
			core::Memory::Free(p7);
			core::Memory::Free(p8);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			if (!ari::net::Serialize<P3, Stream>(stream, *p3))
				return false;
			if (!ari::net::Serialize<P4, Stream>(stream, *p4))
				return false;
			if (!ari::net::Serialize<P5, Stream>(stream, *p5))
				return false;
			if (!ari::net::Serialize<P6, Stream>(stream, *p6))
				return false;
			if (!ari::net::Serialize<P7, Stream>(stream, *p7))
				return false;
			if (!ari::net::Serialize<P8, Stream>(stream, *p8))
				return false;
			return true;
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
			Fn(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void SetParam3(void* param) override
		{
			*p3 = *(P3*)param;
		}

		void SetParam4(void* param) override
		{
			*p4 = *(P4*)param;
		}

		void SetParam5(void* param) override
		{
			*p5 = *(P5*)param;
		}

		void SetParam6(void* param) override
		{
			*p6 = *(P6*)param;
		}

		void SetParam7(void* param) override
		{
			*p7 = *(P7*)param;
		}

		void SetParam8(void* param) override
		{
			*p8 = *(P8*)param;
		}

		void(*Fn)(P1, P2, P3, P4, P5, P6, P7, P8);

		P1* p1 = nullptr;
		P2* p2 = nullptr;
		P3* p3 = nullptr;
		P4* p4 = nullptr;
		P5* p5 = nullptr;
		P6* p6 = nullptr;
		P7* p7 = nullptr;
		P8* p8 = nullptr;

	};


	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	class RPC9 : public RPC
	{
	public:

		RPC9()
		{
			p1 = (P1*)core::Memory::Alloc(sizeof(P1));
			p2 = (P2*)core::Memory::Alloc(sizeof(P2));
			p3 = (P3*)core::Memory::Alloc(sizeof(P3));
			p4 = (P4*)core::Memory::Alloc(sizeof(P4));
			p5 = (P5*)core::Memory::Alloc(sizeof(P5));
			p6 = (P6*)core::Memory::Alloc(sizeof(P6));
			p7 = (P7*)core::Memory::Alloc(sizeof(P7));
			p8 = (P8*)core::Memory::Alloc(sizeof(P8));
			p9 = (P9*)core::Memory::Alloc(sizeof(P9));
		}

		~RPC9() override
		{
			core::Memory::Free(p1);
			core::Memory::Free(p2);
			core::Memory::Free(p3);
			core::Memory::Free(p4);
			core::Memory::Free(p5);
			core::Memory::Free(p6);
			core::Memory::Free(p7);
			core::Memory::Free(p8);
			core::Memory::Free(p9);
		}

		template<typename Stream>
		bool SerializeRpc(Stream& stream)
		{
			if (!ari::net::Serialize<P1, Stream>(stream, *p1))
				return false;
			if (!ari::net::Serialize<P2, Stream>(stream, *p2))
				return false;
			if (!ari::net::Serialize<P3, Stream>(stream, *p3))
				return false;
			if (!ari::net::Serialize<P4, Stream>(stream, *p4))
				return false;
			if (!ari::net::Serialize<P5, Stream>(stream, *p5))
				return false;
			if (!ari::net::Serialize<P6, Stream>(stream, *p6))
				return false;
			if (!ari::net::Serialize<P7, Stream>(stream, *p7))
				return false;
			if (!ari::net::Serialize<P8, Stream>(stream, *p8))
				return false;
			if (!ari::net::Serialize<P9, Stream>(stream, *p9))
				return false;
			return true;
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
			Fn(*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9);
		}


		void SetParam1(void* param) override
		{
			*p1 = *(P1*)param;
		}

		void SetParam2(void* param) override
		{
			*p2 = *(P2*)param;
		}

		void SetParam3(void* param) override
		{
			*p3 = *(P3*)param;
		}

		void SetParam4(void* param) override
		{
			*p4 = *(P4*)param;
		}

		void SetParam5(void* param) override
		{
			*p5 = *(P5*)param;
		}

		void SetParam6(void* param) override
		{
			*p6 = *(P6*)param;
		}

		void SetParam7(void* param) override
		{
			*p7 = *(P7*)param;
		}

		void SetParam8(void* param) override
		{
			*p8 = *(P8*)param;
		}

		void SetParam9(void* param) override
		{
			*p9 = *(P9*)param;
		}

		void(*Fn)(P1, P2, P3, P4, P5, P6, P7, P8, P9);

		P1* p1 = nullptr;
		P2* p2 = nullptr;
		P3* p3 = nullptr;
		P4* p4 = nullptr;
		P5* p5 = nullptr;
		P6* p6 = nullptr;
		P7* p7 = nullptr;
		P8* p8 = nullptr;
		P9* p9 = nullptr;

	};


} // namespace ari::net
