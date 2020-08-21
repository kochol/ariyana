
#include "io.h"
#include "io/FileSystem.hpp"
#include "io/Window.hpp"
#include "fs_local/FileSystemLocal.hpp"
#include "gfx/Application.hpp"
#include "io/platform/Platform.hpp"
#include "zip_file.hpp"

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

CARI_API uint32_t GetDeviceID()
{
	return ari::io::GetDeviceID();
}

// File
void LoadFile(char* _path, on_file_load* _onLoad, void* _load_user_data,
	on_file_failed* _onFailed, void* _failed_user_data)
{
	a_assert(_onLoad);

	if (_onFailed)
	{
		ari::io::LoadFile(_path, [_onLoad, _load_user_data](ari::core::Buffer* _buffer) 
			{
				_onLoad(_buffer->Data(), _buffer->Size(), _load_user_data);
			}, [_onFailed, _failed_user_data](ari::io::IOStatus::Code _code) 
			{
				_onFailed((int)_code, _failed_user_data);
			});
	}
	else
	{
		ari::io::LoadFile(_path, [_onLoad, _load_user_data](ari::core::Buffer* _buffer) 
			{
				_onLoad(_buffer->Data(), _buffer->Size(), _load_user_data);
			});
	}
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

// ZIP
uint8_t* Zip_Compress(uint8_t* data, int* size)
{
	std::string vData(data, data + *size);
	std::vector<unsigned char> vOut;

	miniz_cpp::zip_file file;
	file.writestr("ari.file", vData);
	file.save(vOut);
	*size = int(vOut.size());
	uint8_t* buffer = (uint8_t*)ari::core::Memory::Alloc(*size);
	ari::core::Memory::Copy(vOut.data(), buffer, *size);

	return buffer;
}

uint8_t* Zip_Decompress(uint8_t* data, int* size)
{
	std::vector<unsigned char> vData(data, data + *size);

	miniz_cpp::zip_file file(vData);
	auto out = file.read("ari.file");

	*size = int(out.size());
	uint8_t* buffer = (uint8_t*)ari::core::Memory::Alloc(*size);
	ari::core::Memory::Copy(out.data(), buffer, *size);

	return buffer;
}
