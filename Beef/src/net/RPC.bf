using System;

namespace ari
{
	public abstract class RPC
	{
		public abstract bool Serialize(void* stream);
		public abstract bool Deserialize(void* stream);
		public abstract bool SerializeMeasure(void* stream);

		// Call the function
		public abstract void Call();

		// Clone the RPC
		public abstract RPC Clone();

		// Delete the delegate
		public abstract void Delete();

		public virtual void SetParam1(void* param) {}
		public virtual void SetParam2(void* param) {}
		public virtual void SetParam3(void* param) {}
		public virtual void SetParam4(void* param) {}
		public virtual void SetParam5(void* param) {}
		public virtual void SetParam6(void* param) {}
		public virtual void SetParam7(void* param) {}
		public virtual void SetParam8(void* param) {}
		public virtual void SetParam9(void* param) {}

		public String function_name;
		public uint32 function_hash = 0;
		public RpcType rpc_type;
		public bool Reliable = false;
	}

	// RPC0
	public class RPC0 : RPC
	{
		public delegate void fnDel();
		public fnDel fn = null;

		public override bool Serialize(void* stream)
		{
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			return true;
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn();
		}

		public override RPC Clone()
		{
			RPC0 rpc = new RPC0();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	}

	// RPC1
	public class RPC1<P1> : RPC
	{
		public delegate void fnDel(P1 _p1);
		public fnDel fn = null;
		P1 p1;

		public override bool Serialize(void* stream)
		{
			return NetSerializer.Serialize<P1>(stream, p1);
		}

		public override bool Deserialize(void* stream)
		{
			return NetSerializer.Deserialize<P1>(stream, ref p1);
		}

		public override bool SerializeMeasure(void* stream)
		{
			return NetSerializer.SerializeMeasure<P1>(stream, p1);
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1);
		}

		public override RPC Clone()
		{
			RPC1<P1> rpc = new RPC1<P1>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC1
	

	// RPC2
	public class RPC2<P1, P2> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2);
		public fnDel fn = null;
		P1 p1;
		P2 p2;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2);
		}

		public override RPC Clone()
		{
			RPC2<P1, P2> rpc = new RPC2<P1, P2>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC2

	// RPC3
	public class RPC3<P1, P2, P3> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2, P3 _p3);
		public fnDel fn = null;
		P1 p1;
		P2 p2;
		P3 p3;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			if (!NetSerializer.Serialize<P3>(stream, p3))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			if (!NetSerializer.Deserialize<P3>(stream, ref p3))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			if (!NetSerializer.SerializeMeasure<P3>(stream, p3))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void SetParam3(void* param)
		{
			p3 = *((P3*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2, p3);
		}

		public override RPC Clone()
		{
			RPC3<P1, P2, P3> rpc = new RPC3<P1, P2, P3>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC3

	// RPC4
	public class RPC4<P1, P2, P3, P4> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2, P3 _p3, P4 _p4);
		public fnDel fn = null;
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			if (!NetSerializer.Serialize<P3>(stream, p3))
				return false;
			if (!NetSerializer.Serialize<P4>(stream, p4))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			if (!NetSerializer.Deserialize<P3>(stream, ref p3))
				return false;			
			if (!NetSerializer.Deserialize<P4>(stream, ref p4))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			if (!NetSerializer.SerializeMeasure<P3>(stream, p3))
				return false;
			if (!NetSerializer.SerializeMeasure<P4>(stream, p4))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void SetParam3(void* param)
		{
			p3 = *((P3*)param);
		}

		public override void SetParam4(void* param)
		{
			p4 = *((P4*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2, p3, p4);
		}

		public override RPC Clone()
		{
			RPC4<P1, P2, P3, P4> rpc = new RPC4<P1, P2, P3, P4>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC4

	// RPC5
	public class RPC5<P1, P2, P3, P4, P5> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5);
		public fnDel fn = null;
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			if (!NetSerializer.Serialize<P3>(stream, p3))
				return false;
			if (!NetSerializer.Serialize<P4>(stream, p4))
				return false;
			if (!NetSerializer.Serialize<P5>(stream, p5))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			if (!NetSerializer.Deserialize<P3>(stream, ref p3))
				return false;			
			if (!NetSerializer.Deserialize<P4>(stream, ref p4))
				return false;			
			if (!NetSerializer.Deserialize<P5>(stream, ref p5))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			if (!NetSerializer.SerializeMeasure<P3>(stream, p3))
				return false;
			if (!NetSerializer.SerializeMeasure<P4>(stream, p4))
				return false;
			if (!NetSerializer.SerializeMeasure<P5>(stream, p5))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void SetParam3(void* param)
		{
			p3 = *((P3*)param);
		}

		public override void SetParam4(void* param)
		{
			p4 = *((P4*)param);
		}

		public override void SetParam5(void* param)
		{
			p5 = *((P5*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2, p3, p4, p5);
		}

		public override RPC Clone()
		{
			RPC5<P1, P2, P3, P4, P5> rpc = new RPC5<P1, P2, P3, P4, P5>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC5

	// RPC6
	public class RPC6<P1, P2, P3, P4, P5, P6> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5, P6 _p6);
		public fnDel fn = null;
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			if (!NetSerializer.Serialize<P3>(stream, p3))
				return false;
			if (!NetSerializer.Serialize<P4>(stream, p4))
				return false;
			if (!NetSerializer.Serialize<P5>(stream, p5))
				return false;
			if (!NetSerializer.Serialize<P6>(stream, p6))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			if (!NetSerializer.Deserialize<P3>(stream, ref p3))
				return false;			
			if (!NetSerializer.Deserialize<P4>(stream, ref p4))
				return false;			
			if (!NetSerializer.Deserialize<P5>(stream, ref p5))
				return false;			
			if (!NetSerializer.Deserialize<P6>(stream, ref p6))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			if (!NetSerializer.SerializeMeasure<P3>(stream, p3))
				return false;
			if (!NetSerializer.SerializeMeasure<P4>(stream, p4))
				return false;
			if (!NetSerializer.SerializeMeasure<P5>(stream, p5))
				return false;
			if (!NetSerializer.SerializeMeasure<P6>(stream, p6))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void SetParam3(void* param)
		{
			p3 = *((P3*)param);
		}

		public override void SetParam4(void* param)
		{
			p4 = *((P4*)param);
		}

		public override void SetParam5(void* param)
		{
			p5 = *((P5*)param);
		}

		public override void SetParam6(void* param)
		{
			p6 = *((P6*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2, p3, p4, p5, p6);
		}

		public override RPC Clone()
		{
			RPC6<P1, P2, P3, P4, P5, P6> rpc = new RPC6<P1, P2, P3, P4, P5, P6>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC6

	// RPC7
	public class RPC7<P1, P2, P3, P4, P5, P6, P7> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5, P6 _p6, P7 _p7);
		public fnDel fn = null;
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		P7 p7;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			if (!NetSerializer.Serialize<P3>(stream, p3))
				return false;
			if (!NetSerializer.Serialize<P4>(stream, p4))
				return false;
			if (!NetSerializer.Serialize<P5>(stream, p5))
				return false;
			if (!NetSerializer.Serialize<P6>(stream, p6))
				return false;
			if (!NetSerializer.Serialize<P7>(stream, p7))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			if (!NetSerializer.Deserialize<P3>(stream, ref p3))
				return false;			
			if (!NetSerializer.Deserialize<P4>(stream, ref p4))
				return false;			
			if (!NetSerializer.Deserialize<P5>(stream, ref p5))
				return false;			
			if (!NetSerializer.Deserialize<P6>(stream, ref p6))
				return false;			
			if (!NetSerializer.Deserialize<P7>(stream, ref p7))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			if (!NetSerializer.SerializeMeasure<P3>(stream, p3))
				return false;
			if (!NetSerializer.SerializeMeasure<P4>(stream, p4))
				return false;
			if (!NetSerializer.SerializeMeasure<P5>(stream, p5))
				return false;
			if (!NetSerializer.SerializeMeasure<P6>(stream, p6))
				return false;
			if (!NetSerializer.SerializeMeasure<P7>(stream, p7))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void SetParam3(void* param)
		{
			p3 = *((P3*)param);
		}

		public override void SetParam4(void* param)
		{
			p4 = *((P4*)param);
		}

		public override void SetParam5(void* param)
		{
			p5 = *((P5*)param);
		}

		public override void SetParam6(void* param)
		{
			p6 = *((P6*)param);
		}

		public override void SetParam7(void* param)
		{
			p7 = *((P7*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2, p3, p4, p5, p6, p7);
		}

		public override RPC Clone()
		{
			RPC7<P1, P2, P3, P4, P5, P6, P7> rpc = new RPC7<P1, P2, P3, P4, P5, P6, P7>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC7

	// RPC8
	public class RPC8<P1, P2, P3, P4, P5, P6, P7, P8> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5, P6 _p6, P7 _p7, P8 _p8);
		public fnDel fn = null;
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		P7 p7;
		P8 p8;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			if (!NetSerializer.Serialize<P3>(stream, p3))
				return false;
			if (!NetSerializer.Serialize<P4>(stream, p4))
				return false;
			if (!NetSerializer.Serialize<P5>(stream, p5))
				return false;
			if (!NetSerializer.Serialize<P6>(stream, p6))
				return false;
			if (!NetSerializer.Serialize<P7>(stream, p7))
				return false;
			if (!NetSerializer.Serialize<P8>(stream, p8))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			if (!NetSerializer.Deserialize<P3>(stream, ref p3))
				return false;			
			if (!NetSerializer.Deserialize<P4>(stream, ref p4))
				return false;			
			if (!NetSerializer.Deserialize<P5>(stream, ref p5))
				return false;			
			if (!NetSerializer.Deserialize<P6>(stream, ref p6))
				return false;			
			if (!NetSerializer.Deserialize<P7>(stream, ref p7))
				return false;			
			if (!NetSerializer.Deserialize<P8>(stream, ref p8))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			if (!NetSerializer.SerializeMeasure<P3>(stream, p3))
				return false;
			if (!NetSerializer.SerializeMeasure<P4>(stream, p4))
				return false;
			if (!NetSerializer.SerializeMeasure<P5>(stream, p5))
				return false;
			if (!NetSerializer.SerializeMeasure<P6>(stream, p6))
				return false;
			if (!NetSerializer.SerializeMeasure<P7>(stream, p7))
				return false;
			if (!NetSerializer.SerializeMeasure<P8>(stream, p8))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void SetParam3(void* param)
		{
			p3 = *((P3*)param);
		}

		public override void SetParam4(void* param)
		{
			p4 = *((P4*)param);
		}

		public override void SetParam5(void* param)
		{
			p5 = *((P5*)param);
		}

		public override void SetParam6(void* param)
		{
			p6 = *((P6*)param);
		}

		public override void SetParam7(void* param)
		{
			p7 = *((P7*)param);
		}

		public override void SetParam8(void* param)
		{
			p8 = *((P8*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2, p3, p4, p5, p6, p7, p8);
		}

		public override RPC Clone()
		{
			RPC8<P1, P2, P3, P4, P5, P6, P7, P8> rpc = new RPC8<P1, P2, P3, P4, P5, P6, P7, P8>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC8

	// RPC9
	public class RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9> : RPC
	{
		public delegate void fnDel(P1 _p1, P2 _p2, P3 _p3, P4 _p4, P5 _p5, P6 _p6, P7 _p7, P8 _p8, P9 _p9);
		public fnDel fn = null;
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		P7 p7;
		P8 p8;
		P9 p9;

		public override bool Serialize(void* stream)
		{
			if (!NetSerializer.Serialize<P1>(stream, p1))
				return false;
			if (!NetSerializer.Serialize<P2>(stream, p2))
				return false;
			if (!NetSerializer.Serialize<P3>(stream, p3))
				return false;
			if (!NetSerializer.Serialize<P4>(stream, p4))
				return false;
			if (!NetSerializer.Serialize<P5>(stream, p5))
				return false;
			if (!NetSerializer.Serialize<P6>(stream, p6))
				return false;
			if (!NetSerializer.Serialize<P7>(stream, p7))
				return false;
			if (!NetSerializer.Serialize<P8>(stream, p8))
				return false;
			if (!NetSerializer.Serialize<P9>(stream, p9))
				return false;
			return true;
		}

		public override bool Deserialize(void* stream)
		{
			if (!NetSerializer.Deserialize<P1>(stream, ref p1))
				return false;			
			if (!NetSerializer.Deserialize<P2>(stream, ref p2))
				return false;			
			if (!NetSerializer.Deserialize<P3>(stream, ref p3))
				return false;			
			if (!NetSerializer.Deserialize<P4>(stream, ref p4))
				return false;			
			if (!NetSerializer.Deserialize<P5>(stream, ref p5))
				return false;			
			if (!NetSerializer.Deserialize<P6>(stream, ref p6))
				return false;			
			if (!NetSerializer.Deserialize<P7>(stream, ref p7))
				return false;			
			if (!NetSerializer.Deserialize<P8>(stream, ref p8))
				return false;			
			if (!NetSerializer.Deserialize<P9>(stream, ref p9))
				return false;			
			return true;
		}

		public override bool SerializeMeasure(void* stream)
		{
			if (!NetSerializer.SerializeMeasure<P1>(stream, p1))
				return false;
			if (!NetSerializer.SerializeMeasure<P2>(stream, p2))
				return false;
			if (!NetSerializer.SerializeMeasure<P3>(stream, p3))
				return false;
			if (!NetSerializer.SerializeMeasure<P4>(stream, p4))
				return false;
			if (!NetSerializer.SerializeMeasure<P5>(stream, p5))
				return false;
			if (!NetSerializer.SerializeMeasure<P6>(stream, p6))
				return false;
			if (!NetSerializer.SerializeMeasure<P7>(stream, p7))
				return false;
			if (!NetSerializer.SerializeMeasure<P8>(stream, p8))
				return false;
			if (!NetSerializer.SerializeMeasure<P9>(stream, p9))
				return false;
			return true;
		}

		public override void SetParam1(void* param)
		{
			p1 = *((P1*)param);
		}

		public override void SetParam2(void* param)
		{
			p2 = *((P2*)param);
		}

		public override void SetParam3(void* param)
		{
			p3 = *((P3*)param);
		}

		public override void SetParam4(void* param)
		{
			p4 = *((P4*)param);
		}

		public override void SetParam5(void* param)
		{
			p5 = *((P5*)param);
		}

		public override void SetParam6(void* param)
		{
			p6 = *((P6*)param);
		}

		public override void SetParam7(void* param)
		{
			p7 = *((P7*)param);
		}

		public override void SetParam8(void* param)
		{
			p8 = *((P8*)param);
		}

		public override void SetParam9(void* param)
		{
			p9 = *((P9*)param);
		}

		public override void Call()
		{
			Runtime.Assert(fn != null);
			fn(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		public override RPC Clone()
		{
			RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9> rpc = new RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9>();
			rpc.fn = fn;
			rpc.function_hash = function_hash;
			rpc.Reliable = Reliable;
			rpc.rpc_type = rpc_type;
			return rpc;
		}

		public override void Delete()
		{
			delete fn;
		}
	} // RPC9

}
