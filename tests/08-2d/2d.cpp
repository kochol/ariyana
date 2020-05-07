#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"
#include "en/World.hpp"
#include "2d/RenderSystem2D.hpp"
#include "2d/SceneSystem2D.hpp"
#include "2d/Camera2D.hpp"
#include "2d/Sprite.hpp"
#include "fs_local/FileSystemLocal.hpp"
#include "io/FileSystem.hpp"

class _2dApp : public ari::Application
{
public:

	~_2dApp() = default;

	ari::gfx::GfxSetup* GetGfxSetup() override
	{
		return &m_setup;
	}

	void OnInit() override
	{
		// Add systems
		m_world.AddSystem(&m_renderer);
		m_world.AddSystem(&m_scene_mgr);
		ari::io::RegisterFileSystem("file", &m_file_system_local);


		// Create entity and add sprite and camera
		ari::en::EntityHandle entity = m_world.CreateEntity();
		auto camera = m_world.CreateComponent<ari::en::Camera2D, ari::en::Node2D>();
		camera.Component->Position.x = 0.0f;
		camera.Component->Position.y = 0.0f;

		m_world.AddDerivedComponent<ari::en::Camera2D, ari::en::Node2D>(entity, camera);
		auto sprite = m_world.CreateComponent<ari::en::Sprite, ari::en::Node2D>();
		sprite.Component->Texture = ari::gfx::LoadTexture("res:baboon.png");
		m_pSprite = sprite.Component;

		m_world.AddDerivedComponent<ari::en::Sprite, ari::en::Node2D>(entity, sprite);

	}

	void OnFrame(float _elapsedTime) override
	{
		m_world.Update(_elapsedTime);
	}

	void OnCleanup() override
	{

	}

	void OnEvent(ari::io::ari_event* event, ari::io::WindowHandle _window) override
	{

	}

private:
	ari::gfx::GfxSetup			m_setup;
	ari::en::World				m_world;
	ari::en::RenderSystem2D		m_renderer;
	ari::en::SceneSystem2D		m_scene_mgr;
	ari::io::FileSystemLocal	m_file_system_local;
	ari::en::Sprite			*	m_pSprite = nullptr;


};

ARI_MAIN(_2dApp)

