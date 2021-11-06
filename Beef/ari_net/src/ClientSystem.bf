using System;

namespace ari.net
{
	public class ClientSystem: NetworkSystem
	{
		[CLink]
		static extern void* CreateClientSystem();

		public this()
		{
			m_network_type = SystemNetworkType.Client;
			_obj = CreateClientSystem();
		}

		[CLink]
		static extern void DeleteClientSystem(void* _obj);

		public ~this()
		{
			DeleteClientSystem(_obj);
			_obj = null;
		}

		[CLink]
		static extern void ConnectClientSystem(void* _obj, char8* ip, int port);

		public void Connect(char8* ip, int port)
		{
			ConnectClientSystem(_obj, ip, port);
		}

		[CLink]
		static extern void StopClientSystem(void* _obj);

		public void Stop()
		{
			StopClientSystem(_obj);
		}

		[CLink]
		static extern void CallCRPCClientSystem(void* _obj, void* _rpc, bool _reliable, uint32 _index, RpcType _rpc_type);

		protected override void SendRPC(RPC _rpc, uint32 _index, int client_index)
		{
			CallCRPCClientSystem(_obj, Internal.UnsafeCastToPtr(_rpc), _rpc.Reliable, _index, _rpc.rpc_type);
		}

		[CLink]
		static extern void PlayReplayClientSystem(void* _obj, uint8* data, int32 size);

		public void PlayReplay(uint8* data, int32 size)
		{
			PlayReplayClientSystem(_obj, data, size);
		}

		[CLink]
		static extern void SetReplaySpeedClientSystem(void* _obj, float _speed);

		public void SetReplaySpeed(float _speed)
		{
			SetReplaySpeedClientSystem(_obj, _speed);
		}

		[CLink]
		static extern void SetFastForwardClientSystem(void* _obj, bool _fast_forward);

		public void SetFastForward(bool _enable)
		{
			SetFastForwardClientSystem(_obj, _enable);
		}
	}
}
