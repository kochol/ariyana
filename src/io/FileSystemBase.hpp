#pragma once

namespace ari::io
{
    class FileSystemBase
    {
    public:
    
        virtual ~FileSystemBase() = default;

		virtual void LoadFile(const core::String& _url, std::function<void(core::Buffer*)> OnData,
			std::function<void(IOStatus::Code)> OnFail) = 0;

    };
    
} // namespace ari::io
