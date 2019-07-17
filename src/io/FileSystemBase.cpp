#include "FileSystemBase.hpp"
#include <utility>
#include "core/LockScope.hpp"

namespace ari::io
{
	void FileSystemBase::LoadFile(const URL& _url, std::function<void(core::Buffer*)> OnData, std::function<void(IOStatus::Code)> OnFail)
	{
		core::LockScope lock(&m_lock_queue);
		Request* request = core::Memory::New<Request>();
		request->Url = _url;
		request->OnData = OnData;
		request->OnFail = OnFail;
		m_qRequests.Enqueue(request);
	}

	void FileSystemBase::AddDoneRequest(Request* request)
	{
		core::LockScope lock(&m_lock_done_queue);
		m_qDoneRequests.Enqueue(request);
	}

} // namespace ari::io
