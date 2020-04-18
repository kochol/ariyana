#pragma once
#include "io/IOEvents.hpp"
#include "io/Window.hpp"

namespace ari
{
	namespace gfx
	{
		struct GfxSetup;
	}	

    class Application
    {
    public:

		virtual ~Application() = default;

		virtual gfx::GfxSetup* GetGfxSetup() = 0;

		virtual void OnInit() = 0;

		virtual void OnFrame(float _elapsedTime) = 0;

		virtual void OnCleanup() = 0;

		virtual void OnEvent(io::ari_event* event, io::WindowHandle _window) = 0;

    };
  
} // namespace ari

extern ari::Application* ari_main(int argc, char* argv[]);

#define  ARI_MAIN(clazz) \
ari::Application* ari_main(int argc, char* argv[]) \
{ \
	return ari::core::Memory::New<clazz>(); \
}