#pragma once

#include "core/string/String.hpp"
#include <functional>
#include "core/memory/Buffer.hpp"
#include "IOStatus.hpp"

struct sx_job_context;

namespace ari::io
{
	class URL;
	class FileSystemBase;

	void RegisterFileSystem(const core::String& _scheme, FileSystemBase* _fs);

	void AddAssigns(const core::String& _assign, const core::String& _path);

	core::String ResolveAssigns(const core::String& _path);

	void LoadFile(core::String _path, std::function<void(core::Buffer*)> OnData);

	void LoadFile(const core::String& _path, std::function<void(core::Buffer*)> OnData,
		std::function<void(IOStatus::Code)> OnFail);

	sx_job_context* GetIoJobContext();

} // namespace ari::io
