using System;

namespace ari
{
	public class ServerSystem: NetworkSystem
	{
		[CLink]
		static extern void* CreateServerSystem();

		public this()
		{
			m_network_type = SystemNetworkType.Server;
			_obj = CreateServerSystem();
		}

		[CLink]
		static extern void DeleteServerSystem(void* _obj);

		public ~this()
		{
			DeleteServerSystem(_obj);
			_obj = null;
			delete OnClientConnected;
			delete OnClientDisconnected;
		}

		// Subscribe to client connect and disconnect
		function void client_cb(int32 client_id, void* userData);

		public delegate void OnClientConnectedDelegate(int32 client_id);

		public OnClientConnectedDelegate OnClientConnected = null;

		public delegate void OnClientDisconnectedDelegate(int32 client_id);

		public OnClientDisconnectedDelegate OnClientDisconnected = null;

		static void OnClientConnectedCb(int32 client_id, void* userData)
		{
			var sys = (ServerSystem)Internal.UnsafeCastToObject(userData);
			if (sys.OnClientConnected != null)
				sys.OnClientConnected(client_id);
		}

		static void OnClientDisconnectedCb(int32 client_id, void* userData)
		{
			var sys = (ServerSystem)Internal.UnsafeCastToObject(userData);
			if (sys.OnClientDisconnected != null)
				sys.OnClientDisconnected(client_id);
		}

		[CLink]
		static extern void SetOnClientConnectCb(void* _obj, void* _world, void* _userData,
			client_cb on_connect_cb, client_cb on_disconnect_cb);

		protected override void Configure(World _world)
		{
			base.Configure(_world);
			SetOnClientConnectCb(_obj, _world.[Friend]_obj, Internal.UnsafeCastToPtr(this),
				 => OnClientConnectedCb, => OnClientDisconnectedCb);
		}

		[CLink]
		static extern bool CreateServerServerSystem(void* _obj, char8* ip, int port);

		public bool CreateServer(char8* ip, int port)
		{
			return CreateServerServerSystem(_obj, ip, port);
		}

		[CLink]
		static extern void StopServerSystem(void* _obj);

		public void Stop()
		{
			StopServerSystem(_obj);
		}

		[CLink]
		static extern void CallCRPCServerSystem(void* _obj, void* _rpc, bool _reliable, uint32 _index, RpcType _rpc_type, int client_id);

		protected override void SendRPC(RPC _rpc, uint32 _index, int client_index)
		{
			CallCRPCServerSystem(_obj, Internal.UnsafeCastToPtr(_rpc), _rpc.Reliable, _index, _rpc.rpc_type, client_index);
		}

	}
}
