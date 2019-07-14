#pragma once

#include "core/defines.hpp"
#include "core/Delegate.hpp"
#include "core/string/String.hpp"

namespace ari::io
{
	class FileSystemBase;

    ARI_HANDLE(FileHandle);

	void RegisterFileSystem(const core::String& _scheme, FileSystemBase* _fs);

	void AddAssigns(const core::String& _assign, const core::String& _path);

	FileHandle LoadFile(DelegateOneParam<void, int> OnData);

} // namespace ari::io
