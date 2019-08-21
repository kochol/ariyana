#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"
#include "en/World.hpp"
#include "3d/RenderSystem.hpp"
#include "3d/SceneSystem.hpp"
#include "3d/Camera.hpp"
#include "3d/BoxShape.hpp"
#include "net/ServerSystem.hpp"
#include "net/ClientSystem.hpp"
#include "core/log.h"
#include "en/ComponentManager.hpp"

void RpcServerTest(int i)
{
	log_debug("RpcServerTest %d", i);
}

void RpcClientTest(float i)
{
	log_debug("RpcClientTest %f", i);
}

void RpcMultiCastTest(float i)
{
	log_debug("RpcMultiCastTest %f", i);
}

ari::net::RPC	*	g_rpc_server = nullptr,
				*	g_rpc_client = nullptr,
				*	g_rpc_multicast = nullptr;


struct Client
{
	ari::en::World				m_world;
	ari::en::RenderSystem		m_renderer;
	ari::en::SceneSystem		m_scene_mgr;
	ari::net::ClientSystem		m_client_system;

	void Init()
	{
		// Create a new window
		ari::io::Window window;		
		m_scene_mgr.TargetWindow = m_renderer.TargetWindow = ari::io::CreateAriWindow(window, "Client");

		// Add systems
		m_world.AddSystem(&m_renderer);
		m_world.AddSystem(&m_scene_mgr);
		m_world.AddSystem(&m_client_system);
		m_client_system.Connect(yojimbo::Address("127.0.0.1", 55223));
	}

	void Update(float _elasped)
	{
		m_world.Update(_elasped);
	}

	void Shutdown()
	{
		m_client_system.StopClient();
	}
};

class NetworkApp : public ari::Application
{
public:

	~NetworkApp() = default;

	ari::gfx::GfxSetup* GetGfxSetup() override
	{
		return &m_setup;
	}

	void OnInit() override
	{
		// Init network
		ari::net::InitNetwork();

		// Add systems
		m_world.AddSystem(&m_renderer);
		m_world.AddSystem(&m_scene_mgr);
		m_world.AddSystem(&m_server_system);
		m_server_system.CreateServer(yojimbo::Address("127.0.0.1", 55223));

		// Create entity and add box and camera
		ari::en::EntityHandle entity = m_world.CreateEntity();
		auto camera = m_world.CreateComponent<ari::en::Camera, ari::en::Node3D>();
		camera.Component->Position.x = 3.f;
		camera.Component->Position.y = 3.f;
		camera.Component->Position.z = 3.f;
		camera.Component->Target.z = 0.0f;
		m_world.AddDerivedComponent<ari::en::Camera, ari::en::Node3D>(entity, camera);
		auto box = m_world.CreateComponent<ari::en::BoxShape, ari::en::Node3D>();
		m_pBox = box.Component;
		m_world.AddDerivedComponent<ari::en::BoxShape, ari::en::Node3D>(entity, box);

		// Create PropertyReplicator component to sync the rotation
		auto replicator = m_world.CreateComponent<ari::net::PropertyReplicator>();
		replicator.Component->AddProperty(box, "Rotation");
		m_world.AddComponent(entity, replicator);

		// Add entity to the worlld
		auto en = m_world.GetEntity(entity);
		en->bReplicates = true;
		m_world.AddEntity(entity);

		// Add RPC functions
		g_rpc_server = ari::net::AddRPC("RpcServerTest", ari::net::RpcType::Server, RpcServerTest);
		g_rpc_client = ari::net::AddRPC("RpcClientTest", ari::net::RpcType::Client, RpcClientTest);
		g_rpc_multicast = ari::net::AddRPC("RpcMultiCastTest", ari::net::RpcType::MultiCast, RpcMultiCastTest);
	}

	void OnFrame(float _elapsedTime) override
	{
		if (m_bRotate)
		{
			m_pBox->Rotation.x += 0.3f * _elapsedTime;
			m_pBox->Rotation.y += 0.3f * _elapsedTime;
		}

		m_world.Update(_elapsedTime);
		for (auto c: m_aClients)
		{
			c->Update(_elapsedTime);
		}
	}

	void OnCleanup() override
	{
		// Shutdown Network
		for (auto c : m_aClients)
			c->Shutdown();
		m_server_system.StopServer();
		ari::net::ShutdownNetwork();
	}

	void OnEvent(ari::io::ari_event* event, ari::io::WindowHandle _window) override
	{
		if (event->type == ari::io::ARI_EVENTTYPE_KEY_UP)
		{
			switch (event->key_code)
			{
			case ari::io::ARI_KEYCODE_C:
				{
					Client* client = ari::core::Memory::New<Client>();
					m_aClients.Add(client)->Init();
				}
				break;
			case ari::io::ARI_KEYCODE_S:
				m_bRotate = !m_bRotate;
				break;
			case ari::io::ARI_KEYCODE_Q:
				if (_window.Index > 0)
				m_aClients[_window.Index - 1]->m_client_system.CallRPC(g_rpc_server, 7);
				break;
			case ari::io::ARI_KEYCODE_W:
				if (_window.Index == 0) // On main window
				{
					m_server_system.CallRPC(0, g_rpc_client, 12.5f);
				}
				break;
			case ari::io::ARI_KEYCODE_E:
				if (_window.Index == 0) // On main window
				{
					m_server_system.CallRPC(g_rpc_multicast, 12.5f);
				}
				break;
			}
		}
	}

private:

	ari::gfx::GfxSetup			m_setup;
	ari::en::World				m_world;
	ari::en::RenderSystem		m_renderer;
	ari::en::SceneSystem		m_scene_mgr;
	ari::en::BoxShape		*	m_pBox = nullptr;
	ari::net::ServerSystem		m_server_system;
	ari::core::Array<Client*>	m_aClients;
	bool						m_bRotate = true;
};

ARI_MAIN(NetworkApp)
