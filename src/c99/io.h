#ifndef ARI_IO_H
#define ARI_IO_H

#include "macros.h"

// Global
CARI_API void UpdateIo();
CARI_API bool Run();

// FileSystem
CARI_API void RegisterFileSystemLink(void* _obj, char* _scheme);

// FileSystemLocal
CARI_API void* CreateFileSystemLocal();
CARI_API void DeleteFileSystemLocal(void* _obj);

#endif