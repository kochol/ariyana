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
		m_renderer.TargetWindow = ari::io::CreateAriWindow(window, "Client");

		// Add systems
		m_world.AddSystem(&m_renderer);
		m_world.AddSystem(&m_scene_mgr);
		m_client_system.Connect(yojimbo::Address("127.0.0.1", 55223));
		m_world.AddSystem(&m_client_system);
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
		m_server_system.CreateServer(yojimbo::Address("127.0.0.1", 55223));
		m_world.AddSystem(&m_server_system);

		m_pBox->Position.x = 100.0f;
		auto c = ari::en::ComponentManager::CreateComponent(ari::en::BoxShape::Id, &m_world);
		uint8_t buf[400];
		yojimbo::WriteStream write_stream(yojimbo::GetDefaultAllocator(), buf, 400);
		ari::en::ComponentManager::Serialize(ari::en::BoxShape::Id, write_stream, (void*)m_pBox);
		m_pBox->Position.x = 0.0f;
		m_pBox = (ari::en::BoxShape*)c.Component;
		log_debug("%f", m_pBox->Position.x);
		yojimbo::ReadStream read_stream(yojimbo::GetDefaultAllocator(), buf, 400);
		ari::en::ComponentManager::Deserialize(ari::en::BoxShape::Id, read_stream, c.Component);
		log_debug("%f", m_pBox->Position.x);

	}

	void OnFrame(float _elapsedTime) override
	{
		m_pBox->Rotation.x += 0.3f * _elapsedTime;
		m_pBox->Rotation.y += 0.3f * _elapsedTime;

		m_world.Update(_elapsedTime);
		for (auto c: m_aClients)
		{
			c->Update(_elapsedTime);
		}
	}

	void OnCleanup() override
	{
		// Shutdown Network
		m_server_system.StopServer();
		ari::net::ShutdownNetwork();
	}

	void OnEvent(ari::io::ari_event* event, ari::io::WindowHandle _window) override
	{
		if (event->type == ari::io::ARI_EVENTTYPE_KEY_UP)
		{
			Client* client = ari::core::Memory::New<Client>();
			m_aClients.Add(client)->Init();
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
};

ARI_MAIN(NetworkApp)
