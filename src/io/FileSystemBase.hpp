#pragma once

#include "io/IOStatus.hpp"
#include <functional>
#include "core/memory/Buffer.hpp"
#include "core/string/String.hpp"
#include "core/containers/Queue.hpp"
#include "URL.hpp"
#include "core/SpinLock.hpp"

namespace ari::io
{
	class FileSystemBase
    {
    public:
    
        virtual ~FileSystemBase() = default;

		virtual void LoadFile(const URL& _url, std::function<void(core::Buffer*)> OnData,
			std::function<void(IOStatus::Code)> OnFail);

		/// Call the callbacks here.
		virtual void Update() = 0;

    protected:

        struct Request
        {
			URL									Url;
			std::function<void(core::Buffer*)>  OnData;
			std::function<void(IOStatus::Code)> OnFail;
			core::Buffer						Buffer;
			IOStatus::Code						Code = IOStatus::OK;
        };

		core::SpinLock				m_lock_queue;
		core::Queue<Request*>		m_qRequests;
		core::SpinLock				m_lock_done_queue;
		core::Queue<Request*>		m_qDoneRequests;

		void AddDoneRequest(Request* request);			
    };
    
} // namespace ari::io
