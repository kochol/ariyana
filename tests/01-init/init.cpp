#include "io/Window.hpp"
#include "gfx/Application.hpp"
#include "gfx/gfx.hpp"

class InitApp: public ari::Application
{
public:

	ari::gfx::GfxSetup* GetGfxSetup() override
	{
		return &m_gfx_setup;
	}

	void OnInit() override
	{
		
	}

	void OnFrame(float _elapsedTime) override
	{
		
	}

	void OnCleanup() override
	{
		
	}

	void OnEvent(ari::io::ari_event* event, ari::io::WindowHandle _window) override
	{
		
	}

	ari::gfx::GfxSetup m_gfx_setup;
};

ARI_MAIN(InitApp)
