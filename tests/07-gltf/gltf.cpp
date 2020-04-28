#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"
#include "en/World.hpp"
#include "3d/RenderSystem.hpp"
#include "3d/SceneSystem.hpp"
#include "3d/Camera.hpp"
#include "3d/BoxShape.hpp"
#include "io/FileSystem.hpp"
#include "fs_local/FileSystemLocal.hpp"
#include "3d/3dModel.hpp"

class GltfApp : public ari::Application
{
public:

	~GltfApp() = default;

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

		// Create entity and add box and camera
		ari::en::EntityHandle entity = m_world.CreateEntity();
		auto camera = m_world.CreateComponent<ari::en::Camera, ari::en::Node3D>();
		m_camera = camera;
		camera.Component->Position.x = 3.f;
		camera.Component->Position.y = 2.f;
		camera.Component->Position.z = -3.f;
		camera.Component->Target.x = 0.f;
		camera.Component->Target.y = 0.f;
		camera.Component->Target.z = 0.0f;
		m_world.AddDerivedComponent<ari::en::Camera, ari::en::Node3D>(entity, camera);

		// Load gltf model
		ari::en::Load3dScene("res:Duck.gltf", [this, entity](ari::core::Array<ari::en::ComponentHandle<ari::en::Node3D>> nodes)
			{
				ari::en::Add3dScene(&this->m_world, entity, nodes);
			});
	}

	void OnFrame(float _elapsedTime) override
	{
		static float TotalTime = 0.0f;
		TotalTime += _elapsedTime;
		m_camera.Component->Position.x = sx_sin(TotalTime) * 3;
		m_camera.Component->Position.z = sx_cos(TotalTime) * 3;
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
	ari::io::FileSystemLocal	m_file_system_local;
	ari::en::World				m_world;
	ari::en::RenderSystem		m_renderer;
	ari::en::SceneSystem		m_scene_mgr;
	ari::en::ComponentHandle<ari::en::Camera>
								m_camera;
};

ARI_MAIN(GltfApp)
