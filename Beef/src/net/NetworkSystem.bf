using System;
using System.Collections;

namespace ari
{
	public abstract class NetworkSystem: AriSystem
	{
		protected enum SystemNetworkType
		{
			Server,
			Client,
			DedicatedServer
		}

		protected SystemNetworkType m_network_type;

		static Dictionary<uint32, // function_hash
			Queue<RPC>>	dic_q_rpcs = new Dictionary<uint32, Queue<RPC>>() ~ 
		{
			if (_ != null)
			{
				for (var value in _)
				{
					for (var rpc in value.value)
						delete rpc;
					delete value.value;
				}
				delete _;
			}
		};

		protected static Dictionary<uint32, (int refcount, RPC rpc)> dic_rpcs = new Dictionary<uint32, (int refcount, RPC rpc)>() ~ 
		{
			if (_ != null)
			{
				for (var value in _)
				{
					delete value.value.rpc;
				}
				delete _;
			}
		};

		protected static uint32 rpc_index = 0;

		static RPC GetRpcClone(RPC _rpc, out uint32 _index)
		{
			_index = rpc_index;
			rpc_index++;

			if (!dic_q_rpcs.ContainsKey(_rpc.function_hash))
				dic_q_rpcs.Add(_rpc.function_hash, new Queue<RPC>());

			RPC rpc = null;
			if (dic_q_rpcs[_rpc.function_hash].Count > 0)
				rpc = dic_q_rpcs[_rpc.function_hash].Dequeue();
			if (rpc == null)
				rpc = _rpc.Clone();

			dic_rpcs.Add(_index, (1, rpc));

			return rpc;
		}

		static RPC GetRpc(uint32 _index)
		{
			return dic_rpcs[_index].rpc;
		}

		static void DeleteRpc(uint32 _index)
		{
			var rpc_t = dic_rpcs[_index];
			rpc_t.refcount--;
			if (rpc_t.refcount == 0)
			{
				dic_q_rpcs[rpc_t.rpc.function_hash].Enqueue(rpc_t.rpc);
				dic_rpcs.Remove(_index);
			}
		}

		protected abstract void SendRPC(RPC _rpc, uint32 _index, int client_index);

		void CallRPC_internal(RPC rpc, uint32 _index, int client_index = -1)
		{
			if (client_index == -1)
			{
				Runtime.Assert(rpc.rpc_type != RpcType.Client);
				Runtime.Assert((rpc.rpc_type == RpcType.Server
					&& m_network_type == SystemNetworkType.Client)
					|| rpc.rpc_type == RpcType.MultiCast);
			}
			else
				Runtime.Assert(rpc.rpc_type == RpcType.Client);

			if (rpc.rpc_type == RpcType.MultiCast)
			{
				rpc.Call(); // Also call on sender
				if (m_network_type == SystemNetworkType.Client)
					return;
			}

			SendRPC(rpc, _index, client_index);
		}

		public void CallRPC(RPC _rpc)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC(int client_index, RPC _rpc)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1>(RPC _rpc, P1 p1)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1>(int client_index, RPC _rpc, P1 p1)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2>(RPC _rpc, P1 p1, P2 p2)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2>(int client_index, RPC _rpc, P1 p1, P2 p2)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2, P3>(RPC _rpc, P1 p1, P2 p2, P3 p3)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2, P3>(int client_index, RPC _rpc, P1 p1, P2 p2, P3 p3)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2, P3, P4>(RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2, P3, P4>(int client_index, RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2, P3, P4, P5>(RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2, P3, P4, P5>(int client_index, RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6>(RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6>(int client_index, RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6, P7>(RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			var c7 = p7;
			rpc.SetParam7(&c7);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6, P7>(int client_index, RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			var c7 = p7;
			rpc.SetParam7(&c7);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6, P7, P8>(RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			var c7 = p7;
			rpc.SetParam7(&c7);
			var c8 = p8;
			rpc.SetParam8(&c8);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6, P7, P8>(int client_index, RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			var c7 = p7;
			rpc.SetParam7(&c7);
			var c8 = p8;
			rpc.SetParam8(&c8);
			CallRPC_internal(rpc, index, client_index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6, P7, P8, P9>(RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			var c7 = p7;
			rpc.SetParam7(&c7);
			var c8 = p8;
			rpc.SetParam8(&c8);
			var c9 = p9;
			rpc.SetParam9(&c9);
			CallRPC_internal(rpc, index);
		}

		public void CallRPC<P1, P2, P3, P4, P5, P6, P7, P8, P9>(int client_index, RPC _rpc, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
		{
			uint32 index;
			var rpc = GetRpcClone(_rpc, out index);
			var c1 = p1;
			rpc.SetParam1(&c1);
			var c2 = p2;
			rpc.SetParam2(&c2);
			var c3 = p3;
			rpc.SetParam3(&c3);
			var c4 = p4;
			rpc.SetParam4(&c4);
			var c5 = p5;
			rpc.SetParam5(&c5);
			var c6 = p6;
			rpc.SetParam6(&c6);
			var c7 = p7;
			rpc.SetParam7(&c7);
			var c8 = p8;
			rpc.SetParam8(&c8);
			var c9 = p9;
			rpc.SetParam9(&c9);
			CallRPC_internal(rpc, index, client_index);
		}

	}
}
