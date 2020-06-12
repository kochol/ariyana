using System;
using ari;

namespace t02Network
{
	public class NetworkApp: Application
	{
		World m_world = new World();
		RenderSystem m_renderSystem = new RenderSystem();
		SceneSystem m_sceneSystem = new SceneSystem();
		ServerSystem m_serverSystem = new ServerSystem();
		Entity m_entity;
		Camera m_cam;
		BoxShape m_box;
		PropertyReplicator m_pr;
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

		static int test_i = 0;

		public override void OnInit()
		{
			base.OnInit();

			// Init network
			Net.InitNetwork();

			// Add systems
			m_world.AddSystem(m_renderSystem);
			m_world.AddSystem(m_sceneSystem);
			m_world.AddSystem(m_serverSystem);

			m_serverSystem.CreateServer("127.0.0.1", 55223);

			// Create entity
			m_entity = World.CreateEntity();
			*m_entity.Replicates = true;

			// Add camera
			m_cam = World.CreateCamera();
			m_cam.Position.Set(3.0f);
			m_cam.Target.z = 0.0f;
			m_world.AddComponent(m_entity, m_cam);

			// Add box
			m_box = World.CreateBoxShape();
			m_world.AddComponent(m_entity, m_box);

			// Add PropertyReplicator
			m_pr = World.CreatePropertyReplicator();
			m_world.AddComponent(m_entity, m_pr);
			m_pr.AddProperty(m_box, "Rotation");

			// At last add entity to world
			m_world.AddEntity(m_entity);

			// Add RPCs
			m_rpc_test = Net.AddRPC("RpcTest", .MultiCast, new => RpcTest);
			m_rpc_test2 = Net.AddRPC<int>("RpcTest2", .MultiCast, new => RpcTest2);
			m_rpc_server = Net.AddRPC<int>("RpcServer", .Server, new => RpcServer);
		}

		public override void OnFrame(float _elapsedTime)
		{
			base.OnFrame(_elapsedTime);
			m_box.Rotation.y += 0.9f * _elapsedTime;
			m_box.Rotation.x += 0.9f * _elapsedTime;
			m_world.Update(_elapsedTime);
		}

		public override void OnEvent(ari_event* _event, ref WindowHandle _handle)
		{
			base.OnEvent(_event, ref _handle);
			if (_event.type == ari_event_type.ARI_EVENTTYPE_KEY_DOWN)
			{
				m_serverSystem.CallRPC(m_rpc_test); // Multicast test
				m_serverSystem.CallRPC(m_rpc_test2, test_i++); // Multicast test 2
				m_serverSystem.CallRPC(m_rpc_test2, test_i++); // Multicast test 2
			}
		}

		public override void OnCleanup()
		{
			base.OnCleanup();
			delete m_renderSystem;
			delete m_sceneSystem;
			delete m_entity;
			delete m_cam;
			delete m_box;
			delete m_pr;

			m_serverSystem.Stop();
			delete m_serverSystem;
			Net.ShutdownNetwork();

			delete m_world;
		}
	}
}
