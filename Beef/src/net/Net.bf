using System;
using System.Collections;

namespace ari
{
	public enum RpcType : int32
	{
		Server = 0, // Called by client only runs on server
		Client, // Called by server only runs on particular client
		MultiCast // Called by server and runs on both server and all connected clients
	}

	public static class Net
	{
		static Dictionary<uint32, RPC> g_dRpcs = new Dictionary<uint32, RPC>() ~ DeleteRpcs();
		static void DeleteRpcs()
		{
			if (g_dRpcs != null)
			{
				for (var value in g_dRpcs)
				{
					value.value.Delete();
					delete value.value;
				}
				delete g_dRpcs;
			}
		}

		static void DeleteRPC(uint32 _index)
		{
			NetworkSystem.[Friend]DeleteRpc(_index);
		}

		static void AddRpcRef(uint32 _index)
		{
			NetworkSystem.[Friend]dic_rpcs[_index].refcount++;
		}

		static bool SerializeRPC(void* _stream, void* _rpc, uint32 _index)
		{
			RPC rpc = (RPC)Internal.UnsafeCastToObject(_rpc);
			NetSerializer.Serialize(_stream, rpc.function_hash);
			return rpc.Serialize(_stream);
		}

		static bool SerializeMeasureRPC(void* _stream, void* _rpc, uint32 _index)
		{
			RPC rpc = (RPC)Internal.UnsafeCastToObject(_rpc);
			NetSerializer.SerializeMeasure(_stream, rpc.function_hash);
			return rpc.SerializeMeasure(_stream);
		}

		static bool DerializeRPC(void* _stream, out uint32 _index)
		{
			uint32 rpc_hash = 0;

			NetSerializer.Deserialize(_stream, ref rpc_hash);

			if (!g_dRpcs.ContainsKey(rpc_hash))
				return false;

			RPC orig_rpc = g_dRpcs[rpc_hash];
			RPC rpc = NetworkSystem.[Friend]GetRpcClone(orig_rpc, out _index);
			if (!rpc.Deserialize(_stream))
			{
				NetworkSystem.[Friend]DeleteRpc(_index);
				return false;
			}

			return true;
		}

		static void Call_RPC(uint32 rpc_index)
		{
			RPC rpc = NetworkSystem.[Friend]GetRpc(rpc_index);
			rpc.Call();
		}

		function bool serialize_cb(void* _stream, void* _rpc, uint32 _index);
		function bool deserialize_cb(void* _stream, out uint32 _index);
		function void call_rpc_cb(uint32 rpc_index);

		[CLink]
		public static extern bool InitNetworkLink
			(
				serialize_cb _on_serialize, 
				deserialize_cb _on_deserialize, 
				serialize_cb _on_serialize_measure,
				call_rpc_cb _on_call_rpc,
				call_rpc_cb _on_delete_rpc,
				call_rpc_cb _on_add_ref_rpc
			);

		public static bool InitNetwork()
		{
			return InitNetworkLink
				(=> SerializeRPC,
				=> DerializeRPC,
				=> SerializeMeasureRPC,
				=> Call_RPC,
				=> DeleteRPC,
				=> AddRpcRef);
		}	
		
		[CLink]
		public static extern void ShutdownNetwork();

		[CLink]
		public static extern int32 GetLastRpcClientIndex();

		public static RPC AddRPC(String _function_name, RpcType _rpc_type, RPC0.fnDel _fn, bool _reliable = false)
		{
			RPC0 rpc = new RPC0();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1>(String _function_name, RpcType _rpc_type, RPC1<P1>.fnDel _fn, bool _reliable = false)
		{
			RPC1<P1> rpc = new RPC1<P1>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}


		public static RPC AddRPC<P1, P2>(String _function_name, RpcType _rpc_type, RPC2<P1, P2>.fnDel _fn, bool _reliable = false)
		{
			RPC2<P1, P2> rpc = new RPC2<P1, P2>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1, P2, P3>(String _function_name, RpcType _rpc_type, RPC3<P1, P2, P3>.fnDel _fn, bool _reliable = false)
		{
			RPC3<P1, P2, P3> rpc = new RPC3<P1, P2, P3>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1, P2, P3, P4>(String _function_name, RpcType _rpc_type, RPC4<P1, P2, P3, P4>.fnDel _fn, bool _reliable = false)
		{
			RPC4<P1, P2, P3, P4> rpc = new RPC4<P1, P2, P3, P4>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1, P2, P3, P4, P5>(String _function_name, RpcType _rpc_type, RPC5<P1, P2, P3, P4, P5>.fnDel _fn, bool _reliable = false)
		{
			RPC5<P1, P2, P3, P4, P5> rpc = new RPC5<P1, P2, P3, P4, P5>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1, P2, P3, P4, P5, P6>(String _function_name, RpcType _rpc_type, RPC6<P1, P2, P3, P4, P5, P6>.fnDel _fn, bool _reliable = false)
		{
			RPC6<P1, P2, P3, P4, P5, P6> rpc = new RPC6<P1, P2, P3, P4, P5, P6>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1, P2, P3, P4, P5, P6, P7>(String _function_name, RpcType _rpc_type, RPC7<P1, P2, P3, P4, P5, P6, P7>.fnDel _fn, bool _reliable = false)
		{
			RPC7<P1, P2, P3, P4, P5, P6, P7> rpc = new RPC7<P1, P2, P3, P4, P5, P6, P7>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1, P2, P3, P4, P5, P6, P7, P8>(String _function_name, RpcType _rpc_type, RPC8<P1, P2, P3, P4, P5, P6, P7, P8>.fnDel _fn, bool _reliable = false)
		{
			RPC8<P1, P2, P3, P4, P5, P6, P7, P8> rpc = new RPC8<P1, P2, P3, P4, P5, P6, P7, P8>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}

		public static RPC AddRPC<P1, P2, P3, P4, P5, P6, P7, P8, P9>(String _function_name, RpcType _rpc_type, RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9>.fnDel _fn, bool _reliable = false)
		{
			RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9> rpc = new RPC9<P1, P2, P3, P4, P5, P6, P7, P8, P9>();
			rpc.fn = _fn;
			rpc.function_hash = Hash.HashStringFNV32(_function_name);
			rpc.function_name = _function_name;
			rpc.rpc_type = _rpc_type;
			rpc.Reliable = _reliable;

			g_dRpcs.Add(rpc.function_hash, rpc);

			return rpc;
		}
	}
}
