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
		m_world.AddDerivedComponent<gui::Window, gui::Gui>(entity, window);
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
		ari::gfx::Present();
	}

	void OnCleanup() override
	{
		
	}

	void OnEvent(ari_event* event, io::WindowHandle _window) override
	{
		ari::events::OnInputEvent eve;
		eve.event = event;
		eve.window = _window;
		m_world.emit(eve);
	}

private:

	ari::gfx::GfxSetup			m_setup;
	ari::gui::GuiSystem			m_gui_system;
	ari::en::World				m_world;
};

ARI_MAIN(GuiApp)
