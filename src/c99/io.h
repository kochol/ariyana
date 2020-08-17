#ifndef ARI_IO_H
#define ARI_IO_H

#include "macros.h"
#include "io/IOEvents.hpp"

CARI_HANDLE(WindowHandle)
struct WindowSetup;

// Global
CARI_API void UpdateIo();
CARI_API bool Run();
typedef void(on_event_cb)(ari_event* event, WindowHandle* _window);
CARI_API void SetOnEventCallBack(on_event_cb* _onEvent);
CARI_API WindowHandle CreateAriWindow(WindowSetup* _setup, char* _title);
CARI_API void GetWindowSize(WindowHandle* _handle, int* width, int* height); 
CARI_API uint32_t GetDeviceID();

// FileSystem
CARI_API void RegisterFileSystemLink(void* _obj, char* _scheme);

// FileSystemLocal
CARI_API void* CreateFileSystemLocal();
CARI_API void DeleteFileSystemLocal(void* _obj);

// ZIP
CARI_API uint8_t* Zip_Compress(uint8_t* data, int* in_out_size);
CARI_API uint8_t* Zip_Decompress(uint8_t* data, int* in_out_size);

#endif