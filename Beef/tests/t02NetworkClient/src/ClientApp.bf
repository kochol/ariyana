using System;
using ari;

namespace t02Network
{
	public class ClientApp : Application
	{
		World m_world = new World();
		RenderSystem m_renderSystem = new RenderSystem();
		SceneSystem m_sceneSystem = new SceneSystem();
		ClientSystem m_clientSystem = new ClientSystem();

		RPC m_rpc_test;
		RPC m_rpc_test2;
		RPC m_rpc_server;

		static void RpcTest()
		{
			Console.WriteLine("RPC Test works!");
		}

		static void RpcTest2(int i)
		{
			Console.WriteLine("RPC Test works! {}", i);
		}

		void RpcServer(int i)
		{
			Console.WriteLine("RPC server works! {}", i);
		}

		public override void OnInit()
		{
			base.OnInit();

			// Init network
			Net.InitNetwork();

			// Add systems
			m_world.AddSystem(m_renderSystem);
			m_world.AddSystem(m_sceneSystem);
			m_world.AddSystem(m_clientSystem);

			// Connect to the server
			m_clientSystem.Connect("127.0.0.1", 55223);

			// Add RPCs
			m_rpc_test = Net.AddRPC("RpcTest", RpcType.MultiCast, new => RpcTest);
			m_rpc_test2 = Net.AddRPC<int>("RpcTest2", .MultiCast, new => RpcTest2);
			m_rpc_server = Net.AddRPC<int>("RpcServer", .Server, new => RpcServer);
		}

		public override void OnFrame(float _elapsedTime)
		{
			base.OnFrame(_elapsedTime);
			m_world.Update(_elapsedTime);
		}

		int counter = 0;

		public override void OnEvent(ari_event* _event, ref WindowHandle _handle)
		{
			base.OnEvent(_event, ref _handle);
			if (_event.type == .ARI_EVENTTYPE_KEY_DOWN)
			{
				m_clientSystem.CallRPC(m_rpc_server, counter++);
			}
		}

		public override void OnCleanup()
		{
			base.OnCleanup();
			delete m_renderSystem;
			delete m_sceneSystem;
			m_clientSystem.Stop();
			Net.ShutdownNetwork();
			delete m_clientSystem;
			delete m_world;
		}
	}
}
