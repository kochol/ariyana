
#include "io.h"
#include "io/FileSystem.hpp"
#include "io/Window.hpp"
#include "fs_local/FileSystemLocal.hpp"
#include "gfx/Application.hpp"

extern ari::Application* g_application;
on_event_cb* g_OnEvent = nullptr;

// C Application
class CApp: public ari::Application
{
public:

	ari::gfx::GfxSetup* GetGfxSetup() override
	{
		return nullptr;
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

	void OnEvent(ari_event* event, ari::io::WindowHandle _window) override
	{
		if (g_OnEvent)
            g_OnEvent(event, reinterpret_cast<WindowHandle*>(&_window));
	}
};

// Global
void UpdateIo()
{
    ari::io::Update();
}

bool Run()
{
    return ari::io::Run();
}

void SetOnEventCallBack(on_event_cb* OnEvent)
{
    if (g_application == nullptr)
    {
        g_application = ari::core::Memory::New<CApp>();
    }
    g_OnEvent = OnEvent;
}

WindowHandle CreateAriWindow(WindowSetup* _setup, char* _title)
{
    union { ari::io::WindowHandle cpp; WindowHandle c; } w = 
        { ari::io::CreateAriWindow(*(reinterpret_cast<ari::io::Window*>(_setup)), _title) };
    return w.c;
}

void GetWindowSize(WindowHandle* _handle, int* width, int* height)
{
	const union { WindowHandle c{}; ari::io::WindowHandle cpp; } w = { *_handle };
	const auto r = ari::io::GetWindowSize(w.cpp);
	*width = r.width;
	*height = r.height;
}

// FileSystem
void RegisterFileSystemLink(void* _obj, char* _scheme)
{
    ari::io::RegisterFileSystem(_scheme, reinterpret_cast<ari::io::FileSystemBase*>(_obj));
}

// FileSystemLocal
void* CreateFileSystemLocal()
{
    return ari::core::Memory::New<ari::io::FileSystemLocal>();
}

void DeleteFileSystemLocal(void* _obj)
{
    ari::core::Memory::Delete(reinterpret_cast<ari::io::FileSystemLocal*>(_obj));
}