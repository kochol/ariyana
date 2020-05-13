
#include "io.h"
#include "io/FileSystem.hpp"
#include "io/Window.hpp"
#include "fs_local/FileSystemLocal.hpp"

// Global
void UpdateIo()
{
    ari::io::Update();
}

bool Run()
{
    return ari::io::Run();
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