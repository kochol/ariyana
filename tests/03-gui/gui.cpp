#include "io/Window.hpp"
#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"
#include "gui/GuiSystem.hpp"
#include "en/World.hpp"
#include "en/WorldManager.hpp"
#include "gui/Window.hpp"
#include "gui/Button.hpp"

using namespace ari;

class GuiApp : public ari::Application
{
public:

	~GuiApp() = default;

	ari::gfx::GfxSetup* GetGfxSetup() override
	{
		return &m_setup;
	}

	void OnInit() override
	{
		m_world.AddSystem(&m_gui_system);
		auto entity = m_world.CreateEntity();
		auto window = m_world.CreateComponent<gui::Window, gui::Gui>();
		window.Component->Name = "Test Gui";
		m_world.AddComponent(entity, window);
		auto btn = m_world.CreateComponent<gui::Button, gui::Gui>();
		btn->Label = "Click me!";
		window.Component->AddChild(btn.Component);
	}

	void OnFrame(float _elapsedTime) override
	{
		ari::gfx::BeginDefaultPass();
		m_gui_system.Update(&m_world, _elapsedTime, UpdateState::MainThreadState);
		ari::gfx::EndPass();
		ari::gfx::Commit();
	}

	void OnCleanup() override
	{
		
	}

private:

	ari::gfx::GfxSetup			m_setup;
	ari::gui::GuiSystem			m_gui_system;
	ari::en::World				m_world;
};

ARI_MAIN(GuiApp)
