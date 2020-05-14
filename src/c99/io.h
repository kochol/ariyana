#ifndef ARI_IO_H
#define ARI_IO_H

#include "macros.h"
#include "io/IOEvents.hpp"

CARI_HANDLE(WindowHandle)

// Global
CARI_API void UpdateIo();
CARI_API bool Run();
typedef void(on_event_cb)(ari_event* event, WindowHandle* _window);
CARI_API void SetOnEventCallBack(on_event_cb* _onEvent);

// FileSystem
CARI_API void RegisterFileSystemLink(void* _obj, char* _scheme);

// FileSystemLocal
CARI_API void* CreateFileSystemLocal();
CARI_API void DeleteFileSystemLocal(void* _obj);

#endif