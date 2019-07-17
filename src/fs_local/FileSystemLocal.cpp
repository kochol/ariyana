#include "FileSystemLocal.hpp"
#include "core/LockScope.hpp"
#include "sx/jobs.h"
#include "io/FileSystem.hpp"

namespace ari::io
{
	FileSystemLocal* fs_local_instance = nullptr;

	FileSystemLocal::FileSystemLocal()
	{
		fs_local_instance = this;
	}

	//------------------------------------------------------------------------------
	void FileSystemLocal::Update()
	{
		Request* request = nullptr;
		m_lock_queue.Lock();
		if (m_qRequests.Size() > 0)
			request = m_qRequests.Dequeue();
		m_lock_queue.UnLock();

		// Create IO job
		if (request)
		{
			sx_job_desc desc;
			desc.callback = ReadFileJob;
			desc.priority = SX_JOB_PRIORITY_NORMAL;
			desc.user = reinterpret_cast<void*>(request);
			sx_job_t job = sx_job_dispatch(GetIoJobContext(), &desc, 1);
			m_aJobs.Add(job);
		}

		// Check the finished jobs.
		for(int i = 0; i < m_aJobs.Size(); ++i)
		{
			if (sx_job_test_and_del(GetIoJobContext(), m_aJobs[i]))
			{
				m_aJobs.Erase(i);
				i--;
			}
		}

		// Check for finished requests
		while (true)
		{
			m_lock_done_queue.Lock();
			if (m_qDoneRequests.Size() > 0)
			{
				request = m_qDoneRequests.Dequeue();
				m_lock_done_queue.UnLock();

				// Call the user callbacks
				if (request->Buffer.Size() == 0)
				{
					// Call the error cb
					request->OnFail(request->Code);
				}
				else
				{
					// call On data cb
					request->OnData(&request->Buffer);
				}
				core::Memory::Delete(request);
			}
			else
			{
				m_lock_done_queue.UnLock();
				break;
			}
		}
	}

	//------------------------------------------------------------------------------
	void FileSystemLocal::ReadFileJob(int _index, void* _userdata)
	{
		auto request = reinterpret_cast<Request*>(_userdata);

#if SX_COMPILER_MSVC
		FILE* pFile;
		fopen_s(&pFile, request->Url.AsCStr(), "r");
#else
		FILE* pFile = fopen(request->Url.AsCStr(), "r");
#endif
		if (pFile == nullptr)
		{
			// File not found
			request->Code = IOStatus::NotFound;
			fs_local_instance->AddDoneRequest(request);
			return;
		}

		// obtain file size:
		fseek(pFile, 0, SEEK_END);
		long lSize = ftell(pFile);
		rewind(pFile);


		// copy the file into the buffer:
		request->Buffer.Clear();
		request->Buffer.Reserve(int(lSize));
		size_t result = fread((void*)request->Buffer.Data(), 1, lSize, pFile);
		if (result != (size_t)lSize)
		{
			// Reading error
			request->Code = IOStatus::DownloadError;
			fs_local_instance->AddDoneRequest(request);
			return;
		}

		/* the whole file is now loaded in the memory buffer. */

		// terminate
		fclose(pFile);

		// Add the request to the done queue.
		fs_local_instance->AddDoneRequest(request);
	}

} // namespace ari::io
